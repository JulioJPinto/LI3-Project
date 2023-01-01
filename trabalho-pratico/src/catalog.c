#include "catalog.h"

#include "catalog/catalog_user.h"
#include "catalog/catalog_driver.h"
#include "catalog/catalog_ride.h"

#include "price_util.h"
#include "catalog_sort.h"

/**
 * Struct that represents a catalog.
 */
struct Catalog {
    CatalogUser *catalog_user;
    CatalogDriver *catalog_driver;
    CatalogRide *catalog_ride;
};

Catalog *create_catalog(void) {
    Catalog *catalog = malloc(sizeof(struct Catalog));

    catalog->catalog_user = create_catalog_user();
    catalog->catalog_driver = create_catalog_driver();
    catalog->catalog_ride = create_catalog_ride();

    return catalog;
}

void free_catalog(Catalog *catalog) {
    free_catalog_user(catalog->catalog_user);
    free_catalog_driver(catalog->catalog_driver);
    free_catalog_ride(catalog->catalog_ride);

    free(catalog);
}

static inline void internal_parse_and_register_user(Catalog *catalog, char *line, char separator) {
    User *user = parse_line_user(line, separator);
    if (user == NULL) return;

    catalog_user_register_user(catalog->catalog_user, user);
}

void parse_and_register_user(void *catalog, char *line, char separator) {
    internal_parse_and_register_user(catalog, line, separator);
}

static inline void internal_parse_and_register_driver(Catalog *catalog, char *line, char separator) {
    Driver *driver = parse_line_driver(line, separator);
    if (driver == NULL) return;

    catalog_driver_register_driver(catalog->catalog_driver, driver);
}

void parse_and_register_driver(void *catalog, char *line, char separator) {
    internal_parse_and_register_driver(catalog, line, separator);
}

static inline void internal_parse_and_register_ride(Catalog *catalog, char *line, char separator) {
    Ride *ride = parse_line_ride(line, separator);
    if (ride == NULL) return;

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

    char *user_username = ride_get_user_username(ride);

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
        char *city = ride_get_city(ride);
        char *driver_name = driver_get_name(driver);
        catalog_driver_register_driver_ride(catalog->catalog_driver, driver_id, driver_name, driver_score, city);
        free(driver_name);
        free(city);
    }

    if (driver_account_status == ACTIVE && user_account_status == ACTIVE) { // We only need to index for query 8 if both driver and user is active
        Gender user_gender = user_get_gender(user);
        Gender driver_gender = driver_get_gender(driver);
        if (user_gender == driver_gender) {
            int ride_id = ride_get_id(ride);

            Date user_account_creation_date = user_get_account_creation_date(user);
            Date driver_account_creation_date = driver_get_account_creation_date(driver);

            RideDriverAndUserInfo *ride_driver_and_user_info = create_rduinfo(ride_id,
                                                                              user_account_creation_date,
                                                                              driver_account_creation_date);

            catalog_ride_register_rduinfo_same_gender(catalog->catalog_ride, user_gender, ride_driver_and_user_info);
        }
    }

    free(user_username);
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

Ride *catalog_get_ride(Catalog *catalog, int id) {
    return catalog_ride_get_ride(catalog->catalog_ride, id);
}

gboolean catalog_city_exists(Catalog *catalog, char *city) {
    return catalog_ride_city_has_rides(catalog->catalog_ride, city);
}

int query_2_catalog_get_top_drivers_with_best_score(Catalog *catalog, int n, GPtrArray *result) {
    return catalog_driver_get_top_n_drivers_with_best_score(catalog->catalog_driver, n, result);
}

int query_3_catalog_get_top_users_with_longest_total_distance(Catalog *catalog, int n, GPtrArray *result) {
    return catalog_user_get_top_n_users(catalog->catalog_user, n, result);
}

double query_4_catalog_get_average_price_in_city(Catalog *catalog, char *city) {
    return catalog_ride_get_average_price_in_city(catalog->catalog_ride, city);
}

double query_5_catalog_get_average_price_in_date_range(Catalog *catalog, Date start_date, Date end_date) {
    return catalog_ride_get_average_distance_in_date_range(catalog->catalog_ride, start_date, end_date);
}

double query_6_catalog_get_average_distance_in_city_by_date(Catalog *catalog, Date start_date, Date end_date, char *city) {
    return catalog_ride_get_average_distance_in_city_and_date_range(catalog->catalog_ride, start_date, end_date, city);
}

int query_7_catalog_get_top_n_drivers_in_city(Catalog *catalog, int n, char *city, GPtrArray *result) {
    return catalog_driver_get_top_n_drivers_with_best_score_by_city(catalog->catalog_driver, city, n, result);
}

int query_8_catalog_get_rides_with_user_and_driver_with_same_gender_above_acc_age(Catalog *catalog, GPtrArray *result, Gender gender, int min_account_age) {
    return catalog_ride_get_rides_with_user_and_driver_with_same_age_above_acc_age(catalog->catalog_ride, result, gender, min_account_age);
}

void query_9_catalog_get_passengers_that_gave_tip_in_date_range(Catalog *catalog, GPtrArray *result, Date start_date, Date end_date) {
    catalog_ride_get_passengers_that_gave_tip_in_date_range(catalog->catalog_ride, start_date, end_date, result);
}

void notify_stop_registering(Catalog *catalog) {
    catalog_driver_notify_stop_registering(catalog->catalog_driver);
    catalog_user_notify_stop_registering(catalog->catalog_user);
    catalog_ride_notify_stop_registering(catalog->catalog_ride);
}
