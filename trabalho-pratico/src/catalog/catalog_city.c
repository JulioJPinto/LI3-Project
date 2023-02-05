#include "catalog/catalog_city.h"

#include "array_util.h"

/**
 * Struct that represents a catalog city.
 */
struct CatalogCity {
    GPtrArray *city_id_to_city_name_array;
    GHashTable *city_name_to_city_id_hashtable;
};

CatalogCity *create_catalog_city(void) {
    CatalogCity *catalog_city = malloc(sizeof(struct CatalogCity));

    catalog_city->city_id_to_city_name_array = g_ptr_array_new();
    catalog_city->city_name_to_city_id_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return catalog_city;
}

void free_catalog_city(CatalogCity *catalog_city) {
    g_ptr_array_free(catalog_city->city_id_to_city_name_array, TRUE);
    g_hash_table_destroy(catalog_city->city_name_to_city_id_hashtable);

    free(catalog_city);
}

char *catalog_city_get_city_name(CatalogCity *catalog, int city_id) {
    char *city_name = g_ptr_array_get_at_index_safe(catalog->city_id_to_city_name_array, city_id);
    return city_name ? g_strdup(city_name) : NULL;
}

int catalog_city_get_city_id(CatalogCity *catalog, char *city) {
    if (!city) return -1;

    gpointer city_id_ptr;
    gboolean found = g_hash_table_lookup_extended(catalog->city_name_to_city_id_hashtable, city, NULL, &city_id_ptr);

    return found ? GPOINTER_TO_INT(city_id_ptr) : -1;
}

int catalog_city_get_or_register_city_id(CatalogCity *catalog, char *city) {
    int city_id = catalog_city_get_city_id(catalog, city);
    if (city_id != -1) return city_id;

    city_id = (int) catalog->city_id_to_city_name_array->len;
    city = g_strdup(city);

    g_ptr_array_set_at_index_safe(catalog->city_id_to_city_name_array, city_id, city);
    g_hash_table_insert(catalog->city_name_to_city_id_hashtable, city, GINT_TO_POINTER(city_id));

    return city_id;
}
