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

    catalog_get_top_n_drivers(catalog, n, result);

    for (int i = 0; i < n; i++) {
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

    catalog_get_longest_n_total_distance(catalog, n, result);

    for (int i = 0; i < n; i++) {
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

    fprintf(output, "%.3f\n", average_price);
}

/**
 * Query 6
 */
void execute_average_distance_per_city_per_date(Catalog *catalog, FILE *output, char **args) {
    char *city_string = args[0];
    char *start_date_string = args[1];
    char *end_date_string = args[2];

    char *city = g_strdup(city_string);

    Date start_date = parse_date(start_date_string);
    Date end_date = parse_date(end_date_string);

    double average_distance = catalog_get_average_distance_in_city_by_date(catalog, start_date, end_date, city);

    fprintf(output, "%.3f\n", average_distance);

 }

 /**
  * Query 7
  */
void execute_query_top_n_drivers_by_city_and_date(Catalog *catalog, FILE *output)
