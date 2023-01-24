#include "catalog/catalog_driver_city_info.h"

#include "benchmark.h"
#include "lazy.h"
#include "sort_util.h"

struct CatalogDriverCityInfo {
    Lazy *lazy_driver_city_info_collection_hashtable;
    // Lazy of GHashTable<key: city (char*), value: DriverCityInfoCollection>
    //GHashTable <City, Struct<GHashTable<driver_id, DriverCityInfo>, GPtrArray<DriverCityInfo>>
};

typedef struct {
    /**
     * Array of all DriverCityInfo structs for the city.
     * This will be sorted by the driver's score once all the rides are registered.
     * This is used to get the best drivers in O(1).
     */
    GPtrArray *driver_city_info_array;
    /**
     * Hash table that maps driver ids to DriverCityInfo structs.
     * This is used to get the DriverCityInfo of a driver id in O(1) for faster insertions.
     * This hashtable is only valid during the insertion of rides, and is freed after the insertion.
     * You can't use this hashtable after the insertion ends (when `catalog_force_eager_indexing` is called).
     */
    GHashTable *driver_city_info_hashtable;
} DriverCityInfoCollection;


void free_driver_city_info_collection(gpointer value) {
    DriverCityInfoCollection *collection = value;
    g_ptr_array_free(collection->driver_city_info_array, TRUE);
    free(collection);
}

void lazy_driver_city_info_collection_hashtable_apply_function(gpointer lazy_value) {
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, lazy_value);
    BENCHMARK_START(driver_city_info_destroy_and_sort);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        g_timer_start(driver_city_info_destroy_and_sort);
        DriverCityInfoCollection *collection = value;
        // We don't need the hashtable anymore, so we can free it
        g_hash_table_destroy(collection->driver_city_info_hashtable);

        // Sort the array by average score
        sort_array(collection->driver_city_info_array, compare_driver_city_infos_by_average_score);
        BENCHMARK_LOG("driver_city_info_destroy_and_sort (%s): %lf seconds\n", key, g_timer_elapsed(driver_city_info_destroy_and_sort, NULL));
    }
}

CatalogDriverCityInfo *create_catalog_driver_city_info(void) {
    CatalogDriverCityInfo *catalog_driver_city_info = malloc(sizeof(CatalogDriverCityInfo));
    catalog_driver_city_info->lazy_driver_city_info_collection_hashtable =
            lazy_of(g_hash_table_new_full(g_str_hash, g_str_equal, free, free_driver_city_info_collection),
                    lazy_driver_city_info_collection_hashtable_apply_function);
    return catalog_driver_city_info;
}

void free_hash_table(gpointer value) {
    g_hash_table_destroy(value);
}

void free_catalog_driver_city_info(CatalogDriverCityInfo *catalog_driver_city_info) {
    free_lazy(catalog_driver_city_info->lazy_driver_city_info_collection_hashtable, free_hash_table);
    free(catalog_driver_city_info);
}

void catalog_driver_city_info_register(CatalogDriverCityInfo *catalog, int driver_id, char *driver_name, int driver_score, char *ride_city) {
    GHashTable *driver_city_info_collection_hashtable = lazy_get_raw_value(catalog->lazy_driver_city_info_collection_hashtable);
    DriverCityInfoCollection *driver_city_collection = g_hash_table_lookup(driver_city_info_collection_hashtable, ride_city);

    DriverCityInfo *target;
    if (driver_city_collection == NULL) { // ride_city is not in the hashtable
        driver_city_collection = malloc(sizeof(DriverCityInfoCollection));
        driver_city_collection->driver_city_info_array = g_ptr_array_new_with_free_func(free_driver_city_info_voidp);
        driver_city_collection->driver_city_info_hashtable = g_hash_table_new(g_direct_hash, g_direct_equal);

        g_hash_table_insert(driver_city_info_collection_hashtable, g_strdup(ride_city), driver_city_collection);
        goto register_driver_city_info; // We can skip the lookup because we know it's not in the hashtable
    }

    target = g_hash_table_lookup(driver_city_collection->driver_city_info_hashtable, GINT_TO_POINTER(driver_id));

    if (target == NULL) { // driver is not yet registered in city
    register_driver_city_info:
        target = create_driver_city_info(driver_id, driver_name);

        g_ptr_array_add(driver_city_collection->driver_city_info_array, target);
        g_hash_table_insert(driver_city_collection->driver_city_info_hashtable, GINT_TO_POINTER(driver_id), target);
    }

    driver_city_info_register_ride_score(target, driver_score);
}

void catalog_driver_city_info_force_eager_indexing(CatalogDriverCityInfo *catalog) {
    lazy_apply_function(catalog->lazy_driver_city_info_collection_hashtable);
}

int catalog_driver_city_info_get_top_best_drivers_by_city(CatalogDriverCityInfo *catalog, char *city, int n, GPtrArray *result) {
    DriverCityInfoCollection *driver_city_info_collection =
            g_hash_table_lookup(lazy_get_value(catalog->lazy_driver_city_info_collection_hashtable), city);
    if (driver_city_info_collection == NULL) return 0; // city doesn't exist

    GPtrArray *top_drivers_in_city = driver_city_info_collection->driver_city_info_array;

    int size = MIN(n, (int) top_drivers_in_city->len);

    for (int i = 0; i < size; i++) {
        DriverCityInfo *driver = g_ptr_array_index(top_drivers_in_city, i);
        g_ptr_array_add(result, driver);
    }

    return size;
}
