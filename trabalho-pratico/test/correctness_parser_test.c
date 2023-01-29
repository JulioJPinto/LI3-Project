#include "file_util.h"
#include "parser.h"
#include "user.h"
#include "driver.h"
#include "ride.h"

/**
 * Number of lines that failed to parse.
*/
static int failed_lines = 0;

/**
 * Parses an user line and ensures that it returns NULL.
 */
void parse_user_and_check_for_null(void *first_arg, char *line, char separator) {
    (void) first_arg;
    char *new_line = g_strdup(line);
    User *user = parse_line_user(new_line, separator);
    if (user != NULL) {
        fprintf(stderr, "User should've been NULL for line '%s'\n", line);
        failed_lines++;
        free(user);
    }
    free(new_line);
}

/**
 * Parses a driver line and ensures that it returns NULL.
 */
void parse_driver_and_check_for_null(void *first_arg, char *line, char separator) {
    (void) first_arg;
    char *new_line = g_strdup(line);
    Driver *driver = parse_line_driver(new_line, separator);
    if (driver != NULL) {
        fprintf(stderr, "Driver should've been NULL for line '%s'\n", line);
        failed_lines++;
        free(driver);
    }
    free(new_line);
}

/**
 * Parses a ride line and ensures that it returns NULL.
 */
void parse_ride_and_check_for_null(void *first_arg, char *line, char separator) {
    (void) first_arg;
    char *new_line = g_strdup(line);
    Ride *ride = parse_line_ride(new_line, separator);
    if (ride != NULL) {
        fprintf(stderr, "Ride should've been NULL for line '%s'\n", line);
        failed_lines++;
        free(ride);
    }
    free(new_line);
}

/**
 * Parses an user line and ensures that it does not return NULL.
 */
void parse_user_and_check_for_non_null(void *first_arg, char *line, char separator) {
    (void) first_arg;
    char *new_line = g_strdup(line);
    User *user = parse_line_user(new_line, separator);
    if (user == NULL) {
        fprintf(stderr, "User should've been non-NULL for line '%s'\n", line);
        failed_lines++;
    } else {
        free(user);
    }
    free(new_line);
}

/**
 * Parses a driver line and ensures that it does not return NULL.
 */
void parse_driver_and_check_for_non_null(void *first_arg, char *line, char separator) {
    (void) first_arg;
    char *new_line = g_strdup(line);
    Driver *driver = parse_line_driver(new_line, separator);
    if (driver == NULL) {
        fprintf(stderr, "Driver should've been non-NULL for line '%s'\n", line);
        failed_lines++;
    } else {
        free(driver);
    }
    free(new_line);
}

/**
 * Parses a ride line and ensures that it does not return NULL.
 */
void parse_ride_and_check_for_non_null(void *first_arg, char *line, char separator) {
    (void) first_arg;
    char *new_line = g_strdup(line);
    Ride *ride = parse_line_ride(new_line, separator);
    if (ride == NULL) {
        fprintf(stderr, "Ride should've been non-NULL for line '%s'\n", line);
        failed_lines++;
    } else {
        free(ride);
    }
    free(new_line);
}

/**
 * Asserts that the given dataset folder path having CSV files with invalid only lines does not load any user, driver or ride from the CSVs.
 */
void assert_invalid_csv_loads_nothing(char *dataset_folder_path) {
    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    failed_lines = 0;

    read_file(users_file, parse_user_and_check_for_null, NULL);
    read_file(drivers_file, parse_driver_and_check_for_null, NULL);
    read_file(rides_file, parse_ride_and_check_for_null, NULL);

    if (failed_lines) {
        g_test_fail_printf("Failed for %d lines", failed_lines);
    }
}

/**
 * Asserts that no user, driver or ride is loaded from `datasets/data-regular-errors-invalid`.
 */
void assert_invalid_csv_loads_nothing_regular(void) {
    assert_invalid_csv_loads_nothing("datasets/data-regular-errors-invalid");
}

/**
 * Asserts that no user, driver or ride is loaded from `datasets/data-large-errors-invalid`.
 */
void assert_invalid_csv_loads_nothing_large(void) {
    assert_invalid_csv_loads_nothing("datasets/data-large-errors-invalid");
}

/**
 * Asserts that CSVs in `datasets/data-regular-errors-valid` that have valid lines only loads all users, drivers and rides from the CSV.
 */
void assert_valid_csv_loads_everything_regular(void) {
    char *dataset_folder_path = "datasets/data-regular-errors-valid";

    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    failed_lines = 0;

    read_file(users_file, parse_user_and_check_for_non_null, NULL);
    read_file(drivers_file, parse_driver_and_check_for_non_null, NULL);
    read_file(rides_file, parse_ride_and_check_for_non_null, NULL);

    if (failed_lines) {
        g_test_fail_printf("Failed for %d lines", failed_lines);
    }
}
