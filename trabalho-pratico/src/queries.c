#include "queries.h"

#define UNUSED(x) (void) (x)

/**
 * Same as fprintf but only print to stream if DEBUG macro is defined.
 */
void fprintf_debug(FILE *stream, const char *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    fprintf(stream, "(empty) ");
    vfprintf(stream, format, args);
    va_end(args);
#else
    UNUSED(stream);
    UNUSED(format);
#endif
}

/**
 * Query 1 for users
 */
void execute_query_find_user_by_name(Catalog *catalog, FILE *output, char *username) {
    User *user = catalog_get_user(catalog, username);

    if (user == NULL) {
        fprintf_debug(output, "User %s not found\n", username);
        return;
    }

    if (user_get_account_status(user) == INACTIVE) {
        fprintf_debug(output, "User %s is inactive\n", username);
        return;
    }

    char *name = user_get_name(user);
    const char *gender = user_get_gender(user) == F ? "F" : "M";
    int age = get_age(user_get_birthdate(user));
    double average_score = user_get_average_score(user);
    int number_of_rides = user_get_number_of_rides(user);
    double total_spent = user_get_total_spent(user);

    fprintf(output, "%s;%s;%d;%.3f;%d;%.3f\n", name, gender, age, average_score, number_of_rides, total_spent);

    free(name);
}

/**
 * Query 1 for driver
 */
void execute_query_find_driver_by_id(Catalog *catalog, FILE *output, int id) {
    Driver *driver = catalog_get_driver(catalog, id);

    if (driver == NULL) {
        fprintf_debug(output, "Driver %d not found\n", id);
        return;
    }

    if (driver_get_account_status(driver) == INACTIVE) {
        fprintf_debug(output, "Driver %d is inactive\n", id);
        return;
    }

    char *name = driver_get_name(driver);
    const char *gender = driver_get_gender(driver) == F ? "F" : "M";
    int age = get_age(driver_get_birthdate(driver));
    double average_score = driver_get_average_score(driver);
    int number_of_rides = driver_get_number_of_rides(driver);
    double total_spent = driver_get_total_earned(driver);

    fprintf(output, "%s;%s;%d;%.3f;%d;%.3f\n", name, gender, age, average_score, number_of_rides, total_spent);

    free(name);
}

/**
 * Query 1
 */
void execute_query_find_user_or_driver_by_name_or_id(Catalog *catalog, FILE *output, char **args) {
    char *id_or_username = args[0];

    char *rest;
    int id = (int) strtol(id_or_username, &rest, 10);
    if (*rest != '\0') {
        execute_query_find_user_by_name(catalog, output, id_or_username);
    } else {
        execute_query_find_driver_by_id(catalog, output, id);
    }
}

/**
 * Query 2
 */
void execute_query_top_n_drivers(Catalog *catalog, FILE *output, char **args) {
    char *end_ptr;
    int n = (int) strtol(args[0], &end_ptr, 10);
    if (*end_ptr != '\0') {
        fprintf_debug(output, "Couldn't parse number of drivers '%s'\n", args[0]);
        return;
    }

    GPtrArray *result = g_ptr_array_sized_new(n);

    int result_size = catalog_get_top_drivers_with_best_score(catalog, n, result);

    for (int i = 0; i < result_size; i++) {
        Driver *driver = g_ptr_array_index(result, i);

        int id = driver_get_id(driver);
        char *name = driver_get_name(driver);
        double average_score = driver_get_average_score(driver);

        fprintf(output, "%012d;%s;%.3f\n", id, name, average_score);

        free(name);
    }

    g_ptr_array_free(result, TRUE);
}

/**
 * Query 3
 */
void execute_query_longest_n_total_distance(Catalog *catalog, FILE *output, char **args) {
    char *end_ptr;
    int n = (int) strtol(args[0], &end_ptr, 10);
    if (*end_ptr != '\0') {
        fprintf_debug(output, "Couldn't parse number of drivers '%s'\n", args[0]);
        return;
    }

    GPtrArray *result = g_ptr_array_sized_new(n);

    int result_size = catalog_get_top_users_with_longest_total_distance(catalog, n, result);

    for (int i = 0; i < result_size; i++) {
        User *user = g_ptr_array_index(result, i);

        char *username = user_get_username(user);
        char *name = user_get_name(user);
        int total_distance = user_get_total_distance(user);

        fprintf(output, "%s;%s;%d\n", username, name, total_distance);

        free(username);
        free(name);
    }

    g_ptr_array_free(result, TRUE);
}

/**
 * Query 4
 */
void execute_query_average_price_in_city(Catalog *catalog, FILE *output, char **args) {
    char *city = args[0];

    if (!catalog_city_exists(catalog, city)) {
        fprintf_debug(output, "City %s not found\n", city);
        return;
    }

    double average_price = catalog_get_average_price_in_city(catalog, city);

    fprintf(output, "%.3f\n", average_price);
}

/**
 * Query 5
 */
void execute_query_average_price_in_date_range(Catalog *catalog, FILE *output, char **args) {
    char *start_date_string = args[0];
    char *end_date_string = args[1];

    Date start_date = parse_date(start_date_string);
    Date end_date = parse_date(end_date_string);

    double average_price = catalog_get_average_price_in_date_range(catalog, start_date, end_date);

    if (average_price == -1) {
        fprintf_debug(output, "No rides in date range\n");
        return;
    }

    fprintf(output, "%.3f\n", average_price);
}

/**
 * Query 6
 */
void execute_query_average_distance_in_city_in_date_range(Catalog *catalog, FILE *output, char **args) {
    char *city = args[0];

    if (!catalog_city_exists(catalog, city)) {
        fprintf_debug(output, "City %s not found\n", city);
        return;
    }

    char *start_date_string = args[1];
    char *end_date_string = args[2];

    Date start_date = parse_date(start_date_string);
    Date end_date = parse_date(end_date_string);

    double average_distance = catalog_get_average_distance_in_city_by_date(catalog, start_date, end_date, city);

    fprintf(output, "%.3f\n", average_distance);
}

/**
  * Query 7
  */
void execute_query_top_drivers_in_city_by_average_score(Catalog *catalog, FILE *output, char **args) {
    char *end_ptr;
    int n = (int) strtol(args[0], &end_ptr, 10);
    if (*end_ptr != '\0') {
        fprintf_debug(output, "Couldn't parse number of drivers '%s'\n", args[0]);
        return;
    }

    char *city = args[1];

    GPtrArray *result = g_ptr_array_sized_new(n);
    int size = catalog_get_top_n_drivers_in_city(catalog, n, city, result);

    for (int i = 0; i < size; i++) {
        DriverCityInfo *driver = g_ptr_array_index(result, i);
        int id = driver_city_info_get_id(driver);
        char *name = driver_city_info_get_name(driver);
        double average_score = driver_city_info_get_average_score(driver);

        fprintf(output, "%012d;%s;%.3f\n", id, name, average_score);
    }
    
    g_ptr_array_free(result, TRUE);
}

/**
  * Query 8 
  */
void execute_query_rides_with_users_and_drivers_same_gender_by_account_creation_age(Catalog *catalog, FILE *output, char **args) {
    char *gender_string = args[0];
    Gender gender = parse_gender(gender_string);

    char *end_ptr;
    int min_account_age = (int) strtol(args[1], &end_ptr, 10);
    if (*end_ptr != '\0') {
        fprintf_debug(output, "Couldn't parse number of drivers '%s'\n", args[0]);
        return;
    }

    GPtrArray *result = g_ptr_array_new();
    catalog_get_rides_with_user_and_driver_with_same_gender_above_acc_min_age(catalog, result, gender, min_account_age);

    for (size_t i = 0; i < result->len; i++) {
        Ride *ride = g_ptr_array_index(result, i);

        int driver_id = ride_get_driver_id(ride);
        Driver *driver = catalog_get_driver(catalog, driver_id);
        char *driver_name = driver_get_name(driver);

        char *user_username = ride_get_user_username(ride);
        User *user = catalog_get_user(catalog, user_username);
        char *user_name = user_get_name(user);

        fprintf(output, "%012d;%s;%s;%s\n", driver_id, driver_name, user_username, user_name);

        free(driver_name);
        free(user_username);
        free(user_name);
    }

    g_ptr_array_free(result, TRUE);
}

/**
 * Query 9
 */
void execute_query_passenger_that_gave_tip(Catalog *catalog, FILE *output, char **args) {
    char *start_date_string = args[0];
    char *end_date_string = args[1];

    Date start_date = parse_date(start_date_string);
    Date end_date = parse_date(end_date_string);

    GPtrArray *result = g_ptr_array_new();
    catalog_insert_passengers_that_gave_tip_in_date_range(catalog, result, start_date, end_date);

    for (size_t i = 0; i < result->len; i++) {
        Ride *ride = g_ptr_array_index(result, i);
        int id = ride_get_id(ride);
        Date date = ride_get_date(ride);
        int distance = ride_get_distance(ride);
        char *city = ride_get_city(ride);
        double tip = ride_get_tip(ride);

        fprintf(output, "%012d;%02d/%02d/%02d;%d;%s;%.3f\n", id, date.day, date.month, date.year, distance, city, tip);
        free(city);
    }

    g_ptr_array_free(result, TRUE);
}
