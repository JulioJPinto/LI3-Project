#include "catalog/catalog_driver.h"

#include "catalog_sort.h"

struct CatalogDriver {
    GPtrArray *drivers_array;
    GHashTable *driver_from_id_hashtable;
};

/**
 * Function that wraps free driver to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_driver(gpointer driver) {
    free_driver(driver);
}

CatalogDriver *create_catalog_driver(void) {
    CatalogDriver *catalog_driver = malloc(sizeof(CatalogDriver));
    catalog_driver->drivers_array = g_ptr_array_new_with_free_func(glib_wrapper_free_driver);
    catalog_driver->driver_from_id_hashtable = g_hash_table_new(g_direct_hash, g_direct_equal);
    return catalog_driver;
}

void free_catalog_driver(CatalogDriver *catalog_driver) {
    g_ptr_array_free(catalog_driver->drivers_array, TRUE);
    g_hash_table_destroy(catalog_driver->driver_from_id_hashtable);
    free(catalog_driver);
}

void catalog_driver_register_driver(CatalogDriver *catalog_driver, Driver *driver) {
    g_ptr_array_add(catalog_driver->drivers_array, driver);

    g_hash_table_insert(catalog_driver->driver_from_id_hashtable, GINT_TO_POINTER(driver_get_id(driver)), driver);
}

Driver *catalog_driver_get_driver(CatalogDriver *catalog_driver, int driver_id) {
    return g_hash_table_lookup(catalog_driver->driver_from_id_hashtable, GINT_TO_POINTER(driver_id));
}

int catalog_driver_get_top_n_drivers_with_best_score(CatalogDriver *catalog_driver, int n, GPtrArray *result) {
    int length = MIN(n, (int) catalog_driver->drivers_array->len);

    for (int i = 0; i < length; i++) {
        g_ptr_array_add(result, g_ptr_array_index(catalog_driver->drivers_array, i));
    }

    return length;
}

void catalog_driver_notify_stop_registering(CatalogDriver *catalog_driver) {
    g_ptr_array_sort(catalog_driver->drivers_array, glib_wrapper_compare_drivers_by_score);
}
