#include "catalog.h"
#include "catalog/catalog_driver.h"
#include "catalog/catalog_ride.h"
#include "catalog/catalog_user.h"

#include "benchmark.h"
#include "price_util.h"
#include "array_util.h"

/**
 * Struct that represents a catalog.
 */
struct Catalog {
    CatalogUser *catalog_user;
    CatalogDriver *catalog_driver;
    CatalogRide *catalog_ride;

    GPtrArray *city_id_to_city_name_array;
    GHashTable *city_name_to_city_id_hashtable;
};

Catalog *create_catalog(void) {
    Catalog *catalog = malloc(sizeof(struct Catalog));

    catalog->catalog_user = create_catalog_user();
    catalog->catalog_driver = create_catalog_driver();
    catalog->catalog_ride = create_catalog_ride();

    catalog->city_id_to_city_name_array = g_ptr_array_new();
    catalog->city_name_to_city_id_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return catalog;
}

void free_catalog(Catalog *catalog) {
    free_catalog_user(catalog->catalog_user);
    free_catalog_driver(catalog->catalog_driver);
    free_catalog_ride(catalog->catalog_ride);

    g_ptr_array_free(catalog->city_id_to_city_name_array, TRUE);
    g_hash_table_destroy(catalog->city_name_to_city_id_hashtable);

    free(catalog);
}

char *catalog_get_city_name(Catalog *catalog, int city_id) {
    char *city_name = g_ptr_array_get_at_index_safe(catalog->city_id_to_city_name_array, city_id);
    return city_name ? g_strdup(city_name) : NULL;
}

int catalog_get_city_id(Catalog *catalog, char *city) {
    if (!city) return -1;

    gpointer city_id_ptr;
    gboolean found = g_hash_table_lookup_extended(catalog->city_name_to_city_id_hashtable, city, NULL, &city_id_ptr);

    return found ? GPOINTER_TO_INT(city_id_ptr) : -1;
}

int catalog_get_or_register_city_id(Catalog *catalog, char *city) {
    int city_id = catalog_get_city_id(catalog, city);
    if (city_id != -1) return city_id;

    city_id = (int) catalog->city_id_to_city_name_array->len;
    city = g_strdup(city);

    g_ptr_array_set_at_index_safe(catalog->city_id_to_city_name_array, city_id, city);
    g_hash_table_insert(catalog->city_name_to_city_id_hashtable, city, GINT_TO_POINTER(city_id));

    return city_id;
}

/**
 * Internal function that parses a line and registers the parsed user.
 */
static inline void internal_parse_and_register_user(Catalog *catalog, char *line, char separator) {
    User *user = parse_line_user(line, separator);
    if (user == NULL) return;

    catalog_user_register_user(catalog->catalog_user, user);
}

void parse_and_register_user(void *catalog, char *line, char separator) {
    internal_parse_and_register_user(catalog, line, separator);
}

/**
 * Internal function that parses a line and registers the parsed driver.
 */
static inline void internal_parse_and_register_driver(Catalog *catalog, char *line, char separator) {
    char *city;
    Driver *driver = parse_line_driver_detailed(line, separator, &city);
    if (driver == NULL) return;

    int city_id = catalog_get_or_register_city_id(catalog, city);
    driver_set_city_id(driver, city_id);

    catalog_driver_register_driver(catalog->catalog_driver, driver);
}

void parse_and_register_driver(void *catalog, char *line, char separator) {
    internal_parse_and_register_driver(catalog, line, separator);
}

/**
 * Internal function that parses a line and registers the parsed ride.
 */
static inline void internal_parse_and_register_ride(Catalog *catalog, char *line, char separator) {
    char *city;
    char *user_username;

    Ride *ride = parse_line_ride_detailed(line, separator, &city, &user_username);
    if (ride == NULL) return;
    int city_id = catalog_get_or_register_city_id(catalog, city);
    ride_set_city_id(ride, city_id);

    int driver_id = ride_get_driver_id(ride);
    Driver *driver = catalog_get_driver(catalog, driver_id);
    double price = compute_price(ride_get_distance(ride), driver_get_car_class(driver));
    ride_set_price(ride, price);

    double total_price = ride_get_tip(ride) + price;

    driver_increment_number_of_rides(driver);
    int driver_score = ride_get_score_driver(ride);
    driver_add_score(driver, driver_score);
    driver_add_earned(driver, total_price);
    driver_register_ride_date(driver, ride_get_date(ride));

    User *user = catalog_get_user(catalog, user_username);
    user_increment_number_of_rides(user);
    user_add_score(user, ride_get_score_user(ride));
    user_add_spent(user, total_price);
    user_add_total_distance(user, ride_get_distance(ride));
    user_register_ride_date(user, ride_get_date(ride));

    catalog_ride_register_ride(catalog->catalog_ride, ride);

    AccountStatus driver_account_status = driver_get_account_status(driver);
    AccountStatus user_account_status = user_get_account_status(user);

    if (driver_account_status == ACTIVE) { // We only need to index for query 7 if the driver is active
        catalog_driver_register_driver_ride(catalog->catalog_driver, driver, driver_score, city_id);
    }

    if (driver_account_status == ACTIVE && user_account_status == ACTIVE) { // We only need to index for query 8 if both driver and user is active
        Gender user_gender = user_get_gender(user);
        Gender driver_gender = driver_get_gender(driver);
        if (user_gender == driver_gender) {
            Date user_account_creation_date = user_get_account_creation_date(user);
            Date driver_account_creation_date = driver_get_account_creation_date(driver);

            ride_set_user_account_creation_date(ride, user_account_creation_date);
            ride_set_driver_account_creation_date(ride, driver_account_creation_date);

            catalog_ride_register_ride_same_gender(catalog->catalog_ride, user_gender, ride);
        }
    }
}

void parse_and_register_ride(void *catalog, char *line, char separator) {
    internal_parse_and_register_ride(catalog, line, separator);
}

User *catalog_get_user(Catalog *catalog, char *username) {
    return catalog_user_get_user(catalog->catalog_user, username);
}

Driver *catalog_get_driver(Catalog *catalog, int id) {
    return catalog_driver_get_driver(catalog->catalog_driver, id);
}

int query_2_catalog_get_top_drivers_with_best_score(Catalog *catalog, int n, GPtrArray *result) {
    return catalog_driver_get_top_n_drivers_with_best_score(catalog->catalog_driver, n, result);
}

int query_3_catalog_get_top_users_with_longest_total_distance(Catalog *catalog, int n, GPtrArray *result) {
    return catalog_user_get_top_n_users(catalog->catalog_user, n, result);
}

double query_4_catalog_get_average_price_in_city(Catalog *catalog, int city_id) {
    return catalog_ride_get_average_price_in_city(catalog->catalog_ride, city_id);
}

double query_5_catalog_get_average_price_in_date_range(Catalog *catalog, Date start_date, Date end_date) {
    return catalog_ride_get_average_distance_in_date_range(catalog->catalog_ride, start_date, end_date);
}

double query_6_catalog_get_average_distance_in_city_by_date(Catalog *catalog, Date start_date, Date end_date, int city_id) {
    return catalog_ride_get_average_distance_in_city_and_date_range(catalog->catalog_ride, start_date, end_date, city_id);
}

int query_7_catalog_get_top_n_drivers_in_city(Catalog *catalog, int n, int city_id, GPtrArray *result) {
    return catalog_driver_get_top_n_drivers_with_best_score_by_city(catalog->catalog_driver, city_id, n, result);
}

int query_8_catalog_get_rides_with_user_and_driver_with_same_gender_above_acc_age(Catalog *catalog, GPtrArray *result, Gender gender, int min_account_age) {
    return catalog_ride_get_rides_with_user_and_driver_with_same_age_above_acc_age(catalog->catalog_ride, result, gender, min_account_age);
}

void query_9_catalog_get_passengers_that_gave_tip_in_date_range(Catalog *catalog, GPtrArray *result, Date start_date, Date end_date) {
    catalog_ride_get_passengers_that_gave_tip_in_date_range(catalog->catalog_ride, start_date, end_date, result);
}

void catalog_force_eager_indexing(Catalog *catalog) {
    BENCHMARK_START(load_timer);

    catalog_driver_force_eager_indexing(catalog->catalog_driver);
    catalog_user_force_eager_indexing(catalog->catalog_user);
    catalog_ride_force_eager_indexing(catalog->catalog_ride);

    BENCHMARK_END(load_timer, "Final indexing time:    %f seconds\n");
}
