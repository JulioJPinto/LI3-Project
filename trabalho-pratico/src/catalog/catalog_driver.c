#include "catalog/catalog_driver.h"

#include "benchmark.h"
#include "catalog/catalog_driver_city_info.h"
#include "lazy.h"
#include "array_util.h"

struct CatalogDriver {
    Lazy *lazy_drivers_array;
    GPtrArray *driver_from_id_array; // Index is driver id, value is driver pointer

    CatalogDriverCityInfo *catalog_driver_city_info;
};

/**
 * Function that wraps free driver to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_driver(gpointer driver) {
    free_driver(driver);
}

void sort_array_by_driver_score(void *drivers_array) {
    BENCHMARK_START(sort_drivers_array);
    sort_array(drivers_array, compare_drivers_by_score);
    BENCHMARK_END(sort_drivers_array, "sort_drivers_array: %lf seconds\n");
}

CatalogDriver *create_catalog_driver(void) {
    CatalogDriver *catalog_driver = malloc(sizeof(CatalogDriver));
    GPtrArray *drivers_array = g_ptr_array_new_with_free_func(glib_wrapper_free_driver);
    catalog_driver->lazy_drivers_array = lazy_of(drivers_array, sort_array_by_driver_score);
    catalog_driver->driver_from_id_array = g_ptr_array_new();

    catalog_driver->catalog_driver_city_info = create_catalog_driver_city_info();
    return catalog_driver;
}

void free_g_ptr_array(gpointer array) {
    g_ptr_array_free(array, TRUE);
}

void free_catalog_driver(CatalogDriver *catalog_driver) {
    free_lazy(catalog_driver->lazy_drivers_array, free_g_ptr_array);
    g_ptr_array_free(catalog_driver->driver_from_id_array, TRUE);
    free_catalog_driver_city_info(catalog_driver->catalog_driver_city_info);
    free(catalog_driver);
}

void catalog_driver_register_driver(CatalogDriver *catalog_driver, Driver *driver) {
    GPtrArray *drivers_array = lazy_get_raw_value(catalog_driver->lazy_drivers_array);
    g_ptr_array_add(drivers_array, driver);

    g_ptr_array_set_at_index_safe(catalog_driver->driver_from_id_array, driver_get_id(driver), driver);
}

void catalog_driver_register_driver_ride(CatalogDriver *catalog_driver, Driver *driver, int driver_score, int city_id) {
    catalog_driver_city_info_register(catalog_driver->catalog_driver_city_info, driver_get_id(driver), driver_score, city_id);
}

Driver *catalog_driver_get_driver(CatalogDriver *catalog_driver, int driver_id) {
    return g_ptr_array_get_at_index_safe(catalog_driver->driver_from_id_array, driver_id);
}

int catalog_driver_get_top_n_drivers_with_best_score(CatalogDriver *catalog_driver, int n, GPtrArray *result) {
    GPtrArray *drivers_array = lazy_get_value(catalog_driver->lazy_drivers_array);
    int length = MIN(n, (int) drivers_array->len);

    for (int i = 0; i < length; i++) {
        g_ptr_array_add(result, g_ptr_array_index(drivers_array, i));
    }

    return length;
}

int catalog_driver_get_top_n_drivers_with_best_score_by_city(CatalogDriver *catalog_driver, int city_id, int n, GPtrArray *result) {
    return catalog_driver_city_info_get_top_best_drivers_by_city(catalog_driver->catalog_driver_city_info, city_id, n, result);
}

void catalog_driver_force_eager_indexing(CatalogDriver *catalog_driver) {
    lazy_apply_function(catalog_driver->lazy_drivers_array);
    catalog_driver_city_info_force_eager_indexing(catalog_driver->catalog_driver_city_info);
}
