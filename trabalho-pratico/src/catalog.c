#include "catalog.h"
#include "price_util.h"

struct Catalog {
    GPtrArray *users_array;
    GPtrArray *drivers_array;
    GPtrArray *rides_array;

    GHashTable *user_from_username_hashtable;
    GHashTable *driver_from_id_hashtable;
};

void glib_wrapper_free_user(gpointer user) {
    free_user(user);
}

void glib_wrapper_free_driver(gpointer driver) {
    free_driver(driver);
}

void glib_wrapper_free_ride(gpointer ride) {
    free_ride(ride);
}

Catalog *create_catalog(void) {
    Catalog *catalog = malloc(sizeof(struct Catalog));

    catalog->users_array = g_ptr_array_new_with_free_func(glib_wrapper_free_user);
    catalog->drivers_array = g_ptr_array_new_with_free_func(glib_wrapper_free_driver);
    catalog->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);

    catalog->user_from_username_hashtable = g_hash_table_new(g_str_hash, g_str_equal);
    catalog->driver_from_id_hashtable = g_hash_table_new_full(g_int_hash, g_int_equal, free, NULL);

    return catalog;
}

void register_user(Catalog *catalog, User *user) {
    g_ptr_array_add(catalog->users_array, user);
    g_hash_table_insert(catalog->user_from_username_hashtable, user_get_username(user), user);
}

void register_driver(Catalog *catalog, Driver *driver) {
    g_ptr_array_add(catalog->drivers_array, driver);

    int *key = malloc(sizeof(int));
    *key = driver_get_id(driver);
    // No need to free the key, it will be freed when the Driver is freed

    g_hash_table_insert(catalog->driver_from_id_hashtable, key, driver);
}

void register_ride(Catalog *catalog, Ride *ride) {
    g_ptr_array_add(catalog->rides_array, ride);

    Driver *driver = catalog_get_driver(catalog, ride_get_driver_id(ride));
    double total_price = ride_get_tip(ride) + compute_price(ride_get_distance(ride), driver_get_car_class(driver));

    driver_increment_number_of_rides(driver);
    driver_add_score(driver, ride_get_score_driver(ride));
    driver_add_earned(driver, total_price);
    driver_register_last_ride_date(driver, ride_get_date(ride));

    User *user = catalog_get_user(catalog, ride_get_user_username(ride));
    user_increment_number_of_rides(user);
    user_add_score(user, ride_get_score_user(ride));
    user_add_spent(user, total_price);
    user_add_total_distance(user, ride_get_distance(ride));
    user_set_last_ride_date(user, ride_get_date(ride));
}

void wrapper_voidp_register_user(void *catalog, void *user) {
    register_user((Catalog *) catalog, (User *) user);
}

void wrapper_voidp_register_driver(void *catalog, void *driver) {
    register_driver((Catalog *) catalog, (Driver *) driver);
}

void wrapper_voidp_register_ride(void *catalog, void *ride) {
    register_ride((Catalog *) catalog, (Ride *) ride);
}

User *catalog_get_user(Catalog *catalog, char *username) {
    return g_hash_table_lookup(catalog->user_from_username_hashtable, username);
}

Driver *catalog_get_driver(Catalog *catalog, int id) {
    return g_hash_table_lookup(catalog->driver_from_id_hashtable, &id);
}

void catalog_get_top_n_drivers(Catalog *catalog, int n, GPtrArray *result) {
    for (int i = 0; i < n; i++) {
        g_ptr_array_add(result, g_ptr_array_index(catalog->drivers_array, i));
    }
}

int compare_driver_by_activeness(Driver *a, Driver *b) {
    return (int) driver_get_account_status(a) - (int) driver_get_account_status(b);
}

void catalog_get_longest_n_total_distance(Catalog *catalog, int n, GPtrArray *result) {
    for (int i = 0; i < n; i++) {
        g_ptr_array_add(result, g_ptr_array_index(catalog->users_array, i));
    }
}

int compare_driver_by_id(Driver *a, Driver *b) {
    return driver_get_id(a) - driver_get_id(b);
}

int compare_driver_by_last_ride(Driver *a, Driver *b) {
    return date_compare(driver_get_last_ride_date(a), driver_get_last_ride_date(b));
}

int compare_driver_by_score(Driver *a, Driver *b) {
    double average_score_a = driver_get_average_score(a);
    double average_score_b = driver_get_average_score(b);

    return (average_score_a > average_score_b) - (average_score_a < average_score_b);
}

int glib_wrapper_sort_drivers(gconstpointer a, gconstpointer b) {
    Driver *a_driver = *((Driver **) a);
    Driver *b_driver = *((Driver **) b);

    int by_activeness = compare_driver_by_activeness(a_driver, b_driver);
    if (by_activeness != 0) {
        return by_activeness;
    }

    int by_score = compare_driver_by_score(b_driver, a_driver);
    if (by_score != 0) {
        return by_score;
    }

    int by_last_ride = compare_driver_by_last_ride(b_driver, a_driver);
    if (by_last_ride != 0) {
        return by_last_ride;
    }

    int by_id = compare_driver_by_id(b_driver, a_driver);
    return by_id;
}

int compare_user_by_total_distance(User *a, User *b) {
    int total_distance_a = user_get_total_distance(a);
    int total_distance_b = user_get_total_distance(b);

    return total_distance_a - total_distance_b;
}

int compare_user_by_last_ride(User *a, User *b) {
    Date last_ride_date_a = user_get_most_recent_ride(a);
    Date last_ride_date_b = user_get_most_recent_ride(b);

    return date_compare(last_ride_date_a, last_ride_date_b);
}

int compare_user_by_username(User *a, User *b) {
    char *user_username_a = user_get_username(a);
    char *user_username_b = user_get_username(b);

    return (strcmp(user_username_a, user_username_b));
}

int compare_user_by_activeness(User *a, User *b) {
    AccountStatus acc_status_a = user_get_account_status(a);
    AccountStatus acc_status_b = user_get_account_status(b);

    return acc_status_a - acc_status_b;
}

int glib_wrapper_sort_users(gconstpointer a, gconstpointer b) {
    User *a_user = *((User **) a);
    User *b_user = *((User **) b);

    int by_activeness = compare_user_by_activeness(a_user, b_user);
    if (by_activeness != 0) {
        return by_activeness;
    }


    int by_total_distance = compare_user_by_total_distance(b_user, a_user);
    if (by_total_distance != 0) {
        return by_total_distance;
    }

    int by_last_ride = compare_user_by_last_ride(b_user, a_user);
    if (by_last_ride != 0) {
        return by_last_ride;
    }

    int by_username = compare_user_by_username(b_user, a_user);
    return by_username;
}

void notify_stop_registering(Catalog *catalog) {
    g_ptr_array_sort(catalog->drivers_array, glib_wrapper_sort_drivers);
    g_ptr_array_sort(catalog->users_array, glib_wrapper_sort_users);
}

void free_catalog(Catalog *catalog) {
    g_ptr_array_free(catalog->users_array, TRUE);
    g_ptr_array_free(catalog->drivers_array, TRUE);
    g_ptr_array_free(catalog->rides_array, TRUE);

    g_hash_table_destroy(catalog->user_from_username_hashtable);
    g_hash_table_destroy(catalog->driver_from_id_hashtable);
    free(catalog);
}
