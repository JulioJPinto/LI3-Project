#include "catalog/catalog_user.h"

#include "catalog/lazy.h"
#include "catalog_sort.h"
#include "benchmark.h"

struct CatalogUser {
    Lazy *users_lazy;
    GHashTable *user_from_username_hashtable;
};

/**
 * Function that wraps free user to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_user(gpointer user) {
    free_user(user);
}

void sort_array_by_total_distance(void* users_array) {
    GPtrArray *users_ptr_array = (GPtrArray *) users_array;
    sort_array(users_ptr_array, compare_users_by_total_distance);
}

CatalogUser *create_catalog_user(void) {
    CatalogUser *catalog_user = malloc(sizeof(CatalogUser));

    GPtrArray *users_array = g_ptr_array_new_with_free_func(glib_wrapper_free_user);
    catalog_user->users_lazy = lazy_of(users_array,sort_array_by_total_distance, 0);
    catalog_user->user_from_username_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return catalog_user;
}

void free_catalog_user(CatalogUser *catalog_user) {
    g_hash_table_destroy(catalog_user->user_from_username_hashtable);

    free(catalog_user);
}

void catalog_user_register_user(CatalogUser *catalog_user, User *user) {
    GPtrArray* users_array = (GPtrArray *) get_value(catalog_user->users_lazy);
    g_ptr_array_add(users_array, user);

    char *key = user_get_username(user);
    // No need to free the key, it's freed by the hashtable when the user is removed

    g_hash_table_insert(catalog_user->user_from_username_hashtable, key, user);
}

User *catalog_user_get_user(CatalogUser *catalog_user, char *username) {
    return g_hash_table_lookup(catalog_user->user_from_username_hashtable, username);
}

void catalog_user_notify_stop_registering(CatalogUser *catalog_user) {
<<<<<<< HEAD
    GPtrArray *users_array = *(GPtrArray **) get_value(catalog_user->users_lazy);
    BENCHMARK_START(sort_users_array);
=======
    GPtrArray *users_array = (GPtrArray *) get_value(catalog_user->users_lazy);
>>>>>>> 17d0ba6 (fix: gptrarray casts)
    sort_array(users_array, compare_users_by_total_distance);
    BENCHMARK_END(sort_users_array, "     sort_users_array: %lf seconds\n");    
}

int catalog_user_get_top_n_users(CatalogUser *catalog_user, int n, GPtrArray *result) {
    GPtrArray *users_array = (GPtrArray *) get_value(catalog_user->users_lazy);
    int length = MIN(n, (int) users_array->len);

    for (int i = 0; i < length; i++) {
        g_ptr_array_add(result, g_ptr_array_index(users_array, i));
    }

    return length;
}
