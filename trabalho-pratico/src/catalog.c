#include <printf.h>
#include "catalog.h"
#include "price_util.h"

struct Catalog {
    GPtrArray *users_array;
    GPtrArray *drivers_array;
    GPtrArray *rides_array;

    GHashTable *user_from_username_hashtable; // key: username (char*), value: User*
    GHashTable *driver_from_id_hashtable; // key: driver id (int), value: Driver*

    GHashTable *rides_in_city_hashtable; // key: city (char*), value: GPtrArray of rides
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

void glib_wrapper_ptr_array_free_segment(gpointer array) {
    g_ptr_array_free(array, TRUE);
}

Catalog *create_catalog(void) {
    Catalog *catalog = malloc(sizeof(struct Catalog));

    catalog->users_array = g_ptr_array_new_with_free_func(glib_wrapper_free_user);
    catalog->drivers_array = g_ptr_array_new_with_free_func(glib_wrapper_free_driver);
    catalog->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);

    catalog->user_from_username_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    catalog->driver_from_id_hashtable = g_hash_table_new_full(g_int_hash, g_int_equal, free, NULL);

    catalog->rides_in_city_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, glib_wrapper_ptr_array_free_segment);

    return catalog;
}

void free_catalog(Catalog *catalog) {
    g_ptr_array_free(catalog->users_array, TRUE);
    g_ptr_array_free(catalog->drivers_array, TRUE);
    g_ptr_array_free(catalog->rides_array, TRUE);

    g_hash_table_destroy(catalog->user_from_username_hashtable);
    g_hash_table_destroy(catalog->driver_from_id_hashtable);
    g_hash_table_destroy(catalog->rides_in_city_hashtable);

    free(catalog);
}

void register_user(Catalog *catalog, User *user) {
    g_ptr_array_add(catalog->users_array, user);

    char *key = user_get_username(user);
    // No need to free the key, it's freed by the hashtable when the user is removed

    g_hash_table_insert(catalog->user_from_username_hashtable, key, user);
}

void register_driver(Catalog *catalog, Driver *driver) {
    g_ptr_array_add(catalog->drivers_array, driver);

    int *key = malloc(sizeof(int));
    *key = driver_get_id(driver);
    // No need to free the key, it will be freed when the Driver is freed

    g_hash_table_insert(catalog->driver_from_id_hashtable, key, driver);
}

void catalog_ride_index_city(Catalog *catalog, Ride *ride) {
    char *city = ride_get_city(ride); // Only needs to be freed if the city is already in the hashtable

    GPtrArray *rides_in_city;
    if (g_hash_table_contains(catalog->rides_in_city_hashtable, city)) {
        rides_in_city = g_hash_table_lookup(catalog->rides_in_city_hashtable, city);
        free(city);
    } else {
        rides_in_city = g_ptr_array_new();
        g_hash_table_insert(catalog->rides_in_city_hashtable, city, rides_in_city);
    }

    g_ptr_array_add(rides_in_city, ride);
}

void register_ride(Catalog *catalog, Ride *ride) {
    g_ptr_array_add(catalog->rides_array, ride);

    Driver *driver = catalog_get_driver(catalog, ride_get_driver_id(ride));
    double price = compute_price(ride_get_distance(ride), driver_get_car_class(driver));
    ride_set_price(ride, price);

    double total_price = ride_get_tip(ride) + price;

    driver_increment_number_of_rides(driver);
    driver_add_score(driver, ride_get_score_driver(ride));
    driver_add_earned(driver, total_price);
    driver_register_ride_date(driver, ride_get_date(ride));

    char *username = ride_get_user_username(ride);

    User *user = catalog_get_user(catalog, username);
    user_increment_number_of_rides(user);
    user_add_score(user, ride_get_score_user(ride));
    user_add_spent(user, total_price);
    user_add_total_distance(user, ride_get_distance(ride));
    user_register_ride_date(user, ride_get_date(ride));

    free(username);

    catalog_ride_index_city(catalog, ride);
}

User *catalog_get_user(Catalog *catalog, char *username) {
    return g_hash_table_lookup(catalog->user_from_username_hashtable, username);
}

Driver *catalog_get_driver(Catalog *catalog, int id) {
    return g_hash_table_lookup(catalog->driver_from_id_hashtable, &id);
}

void catalog_get_top_n_drivers(Catalog *catalog, int n, GPtrArray *result) {
    // TODO: todo in catalog_get_longest_n_total_distance
    for (int i = 0; i < n; i++) {
        g_ptr_array_add(result, g_ptr_array_index(catalog->drivers_array, i));
    }
}


void catalog_get_longest_n_total_distance(Catalog *catalog, int n, GPtrArray *result) {
    // TODO: This can be improved, no need to copy the array, just create a new one pointing to the users_array with the length of n
    for (int i = 0; i < n; i++) {
        g_ptr_array_add(result, g_ptr_array_index(catalog->users_array, i));
    }
}

double catalog_get_average_price_in_city(Catalog *p_catalog, char *city) {
    GPtrArray *rides = g_hash_table_lookup(p_catalog->rides_in_city_hashtable, city);
    if (rides == NULL) return 0;

    double total_price = 0;

    // Performance impact of this loop is negligible, even with ~200000 rides per city.
    // We also assume that the user won't ask this query twice for the same city, so no need to cache.

    for (uint i = 0; i < rides->len; i++) {
        Ride *ride = g_ptr_array_index(rides, i);
        total_price += ride_get_price(ride);
    }

    return total_price / rides->len;
}

int compare_driver_by_activeness(Driver *a, Driver *b) {
    return (int) driver_get_account_status(a) - (int) driver_get_account_status(b);
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

    int result = strcmp(user_username_a, user_username_b);

    free(user_username_a);
    free(user_username_b);

    return result;
}

int compare_user_by_activeness(User *a, User *b) {
    return (int) user_get_account_status(a) - (int) user_get_account_status(b);
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

    return compare_driver_by_id(b_driver, a_driver);
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

    return compare_user_by_username(a_user, b_user);
}

void notify_stop_registering(Catalog *catalog) {
    g_ptr_array_sort(catalog->drivers_array, glib_wrapper_sort_drivers);
    g_ptr_array_sort(catalog->users_array, glib_wrapper_sort_users);
}
