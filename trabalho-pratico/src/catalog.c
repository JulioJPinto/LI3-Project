#include "catalog.h"

typedef struct Catalog {
    GHashTable *user_from_username_hashtable;
    GHashTable *driver_from_id_hashtable;
    GHashTable *average_ride_price_from_city;
} Catalog;

typedef struct AccumulativeAverage {
    double total;
    double n;
} AccumulativeAverage;

void register_user(Catalog *catalog, User *user) {
    g_hash_table_insert(catalog->user_from_username_hashtable, user_get_username(user), user);
}

void register_driver(Catalog *catalog, Driver *driver) {
    int key = driver_get_id(driver);
    g_hash_table_insert(catalog->driver_from_id_hashtable, &key, driver);
}

void register_ride(Catalog *catalog, Ride *ride) {
    GHashTable *table = catalog->average_ride_price_from_city;
    GString *city = ride_get_city(ride);
    if (g_hash_table_contains(table, city)) {
        // add price to current
    } else {
        AccumulativeAverage accumulative_average = {
                // compute price
        };
        // trocar struct pra malloc
        g_hash_table_insert(table, city, &accumulative_average);
    }
}

Catalog *create_catalog() {
    Catalog *catalog = malloc(sizeof(Catalog));

    catalog->user_from_username_hashtable = g_hash_table_new((GHashFunc) g_string_hash, (GEqualFunc) g_string_equal);
    catalog->driver_from_id_hashtable = g_hash_table_new(g_int_hash, g_int_equal);
    catalog->average_ride_price_from_city = g_hash_table_new((GHashFunc) g_string_hash, (GEqualFunc) g_string_equal);

    return catalog;
}

void free_catalog(Catalog *catalog) {
    g_hash_table_destroy(catalog->user_from_username_hashtable);
    g_hash_table_destroy(catalog->driver_from_id_hashtable);
    free(catalog);
}

User *get_user_from_username(Catalog *catalog, GString username) {
    return g_hash_table_lookup(catalog->user_from_username_hashtable, &username);
}

Driver *get_driver_from_id(Catalog *catalog, int id) {
    return g_hash_table_lookup(catalog->driver_from_id_hashtable, &id);
}

double get_price_average_from_city(Catalog *catalog, GString city_name) {
    AccumulativeAverage *accumulative_average = g_hash_table_lookup(catalog->average_ride_price_from_city, &city_name);

    return accumulative_average->total / accumulative_average->n;
}
