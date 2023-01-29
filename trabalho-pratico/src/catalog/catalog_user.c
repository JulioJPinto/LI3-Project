#include "catalog/catalog_user.h"

#include "benchmark.h"
#include "lazy.h"
#include "array_util.h"

/**
 * Struct that holds all the users and their indexed information.
 */
struct CatalogUser {
    Lazy *lazy_users_array;
    GHashTable *user_from_username_hashtable;
};

/**
 * Function that wraps free user to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_user(gpointer user) {
    free_user(user);
}

/**
 * Function that sorts the users array by total distance.
 */
static void sort_array_by_total_distance(void *users_array) {
    BENCHMARK_START(sort_users_array);
    sort_array(users_array, compare_users_by_total_distance);
    BENCHMARK_END(sort_users_array, "sort_users_array: %lf seconds\n");
}

CatalogUser *create_catalog_user(void) {
    CatalogUser *catalog_user = malloc(sizeof(CatalogUser));

    catalog_user->lazy_users_array = lazy_of(g_ptr_array_new_with_free_func(glib_wrapper_free_user), sort_array_by_total_distance);
    catalog_user->user_from_username_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return catalog_user;
}

/**
 * Frees the user array.
 */
void free_users_array(gpointer array) {
    g_ptr_array_free(array, TRUE);
}

void free_catalog_user(CatalogUser *catalog_user) {
    g_hash_table_destroy(catalog_user->user_from_username_hashtable);
    free_lazy(catalog_user->lazy_users_array, free_users_array);

    free(catalog_user);
}

void catalog_user_register_user(CatalogUser *catalog_user, User *user) {
    g_ptr_array_add(lazy_get_raw_value(catalog_user->lazy_users_array), user);

    char *key = user_get_username(user);
    // No need to free the key, it's freed by the hashtable when the user is removed

    g_hash_table_insert(catalog_user->user_from_username_hashtable, key, user);
}

User *catalog_user_get_user(CatalogUser *catalog_user, char *username) {
    return g_hash_table_lookup(catalog_user->user_from_username_hashtable, username);
}

void catalog_user_force_eager_indexing(CatalogUser *catalog_user) {
    lazy_apply_function(catalog_user->lazy_users_array);
}

int catalog_user_get_top_n_users(CatalogUser *catalog_user, int n, GPtrArray *result) {
    GPtrArray *users_array = (GPtrArray *) lazy_get_value(catalog_user->lazy_users_array);
    int length = MIN(n, (int) users_array->len);

    for (int i = 0; i < length; i++) {
        g_ptr_array_add(result, g_ptr_array_index(users_array, i));
    }

    return length;
}
