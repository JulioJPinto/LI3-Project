#include "catalog/catalog_driver_city_info.h"

#include "benchmark.h"
#include "lazy.h"
#include "array_util.h"

/**
 * Struct that holds an array of DriverCityInfoCollection for each city.
 */
struct CatalogDriverCityInfo {
    Lazy *lazy_driver_city_info_collection_array;
    //Lazy of GPtrArray<index: city_id, value: DriverCityInfoCollection>
};

/**
 * Struct that holds DriverCityInfo structs for a city.
 */
typedef struct {
    /**
     * Array of all DriverCityInfo structs for the city.
     * This will be sorted by the driver's score once all the rides are registered.
     * This is used to get the best drivers in O(1).
     */
    GPtrArray *driver_city_info_array;
    /**
     * Hash table that maps driver ids to DriverCityInfo structs.
     * We need this because not all drivers will have rides in all cities.
     * This is used to get the DriverCityInfo of a driver id in O(1) (instead of looping through an array) for faster insertions.
     * This hashtable is only valid during the insertion of rides, and is freed after the insertion.
     * You can't use this hashtable after the insertion ends (when `catalog_force_eager_indexing` is called).
     */
    GHashTable *driver_city_info_hashtable;
} DriverCityInfoCollection;

/**
 * Free function for DriverCityInfoCollection.
 */
void free_driver_city_info_collection(gpointer value) {
    // This can happen if the array has not been fully populated with city ids, thus some indexes are NULL.
    if (value == NULL) return;

    DriverCityInfoCollection *collection = value;
    if(collection->driver_city_info_hashtable != NULL) g_hash_table_destroy(collection->driver_city_info_hashtable);
    
    g_ptr_array_free(collection->driver_city_info_array, TRUE);
    free(collection);
}

/**
 * Function that is called when the lazy value is accessed (or when `lazy_force_eager_indexing` is called).
 * For each city, the hash table is freed and the array is sorted by the driver's average score in that city.
 */
void lazy_driver_city_info_collection_array_apply_function(gpointer lazy_value) {
    GPtrArray *driver_city_info_array = lazy_value;

    BENCHMARK_START(driver_city_info_destroy_and_sort);
    for (int i = 0; i < (int) driver_city_info_array->len; i++) {
        DriverCityInfoCollection *collection = g_ptr_array_get_at_index_safe(driver_city_info_array, i);
        if (collection == NULL) continue;

        g_hash_table_destroy(collection->driver_city_info_hashtable);
        collection->driver_city_info_hashtable = NULL;

        sort_array(collection->driver_city_info_array, compare_driver_city_infos_by_average_score);
        BENCHMARK_LOG("driver_city_info_destroy_and_sort (%d): %lf seconds\n", i, g_timer_elapsed(driver_city_info_destroy_and_sort, NULL));
    }
}

CatalogDriverCityInfo *create_catalog_driver_city_info(void) {
    CatalogDriverCityInfo *catalog_driver_city_info = malloc(sizeof(CatalogDriverCityInfo));
    catalog_driver_city_info->lazy_driver_city_info_collection_array =
            lazy_of(g_ptr_array_new_with_free_func(free_driver_city_info_collection),
                    lazy_driver_city_info_collection_array_apply_function);
    return catalog_driver_city_info;
}

/**
 * Frees the array of DriverCityInfoCollection.
*/
void free_array(gpointer value) {
    g_ptr_array_free(value, TRUE);
}

void free_catalog_driver_city_info(CatalogDriverCityInfo *catalog_driver_city_info) {
    free_lazy(catalog_driver_city_info->lazy_driver_city_info_collection_array, free_array);
    free(catalog_driver_city_info);
}

void catalog_driver_city_info_register(CatalogDriverCityInfo *catalog, int driver_id, int driver_score, int city_id) {
    GPtrArray *driver_city_info_collection_array = lazy_get_raw_value(catalog->lazy_driver_city_info_collection_array);
    DriverCityInfoCollection *driver_city_collection = g_ptr_array_get_at_index_safe(driver_city_info_collection_array, city_id);

    DriverCityInfo *target;
    if (driver_city_collection == NULL) { // ride_city is not in the hashtable
        driver_city_collection = malloc(sizeof(DriverCityInfoCollection));
        driver_city_collection->driver_city_info_array = g_ptr_array_new_with_free_func(free_driver_city_info_voidp);
        driver_city_collection->driver_city_info_hashtable = g_hash_table_new(g_direct_hash, g_direct_equal);

        g_ptr_array_set_at_index_safe(driver_city_info_collection_array, city_id, driver_city_collection);
        goto register_driver_city_info; // We can skip the lookup because we know it's not in the hashtable
    }

    target = g_hash_table_lookup(driver_city_collection->driver_city_info_hashtable, GINT_TO_POINTER(driver_id));

    if (target == NULL) { // driver is not yet registered in city
    register_driver_city_info:
        target = create_driver_city_info(driver_id);

        g_ptr_array_add(driver_city_collection->driver_city_info_array, target);
        g_hash_table_insert(driver_city_collection->driver_city_info_hashtable, GINT_TO_POINTER(driver_id), target);
    }

    driver_city_info_register_ride_score(target, driver_score);
}

void catalog_driver_city_info_force_eager_indexing(CatalogDriverCityInfo *catalog) {
    lazy_apply_function(catalog->lazy_driver_city_info_collection_array);
}

int catalog_driver_city_info_get_top_best_drivers_by_city(CatalogDriverCityInfo *catalog, int city_id, int n, GPtrArray *result) {
    GPtrArray *driver_city_info_array = lazy_get_value(catalog->lazy_driver_city_info_collection_array);
    DriverCityInfoCollection *driver_city_info_collection = g_ptr_array_get_at_index_safe(driver_city_info_array, city_id);
    if (driver_city_info_collection == NULL) return 0; // city doesn't exist

    GPtrArray *top_drivers_in_city = driver_city_info_collection->driver_city_info_array;

    int size = MIN(n, (int) top_drivers_in_city->len);

    for (int i = 0; i < size; i++) {
        DriverCityInfo *driver = g_ptr_array_index(top_drivers_in_city, i);
        g_ptr_array_add(result, driver);
    }

    return size;
}
