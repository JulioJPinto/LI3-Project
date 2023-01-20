#include "../include/catalog.h"
#include "../include/file_util.h"

#include "struct_util_test.c"
#include "lazy_test.c"
#include "correctness_query_test.c"
#include "performance_query_test.c"

#include <glib.h>
#include <stdio.h>

void assert_invalid_csv_loads_nothing(char *dataset_folder_path) {
    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    char *line = malloc(sizeof(char) * 1000);

    int failed = 0;

    fgets(line, 1000, users_file); // skip first line
    while (fgets(line, 1000, users_file) != NULL) {
        format_fgets_input_line(line);
        char *line_copy = g_strdup(line);
        User *user = parse_line_user(line_copy, ';');
        if (user != NULL) {
            printf("User should've been NULL for line '%s'\n", line);
            failed++;
            free_user(user);
        }
        free(line_copy);
    }

    fgets(line, 1000, drivers_file); // skip first line
    while (fgets(line, 1000, drivers_file) != NULL) {
        format_fgets_input_line(line);
        char *line_copy = g_strdup(line);
        Driver *driver = parse_line_driver(line_copy, ';');
        if (driver != NULL) {
            printf("Driver should've been NULL for line '%s'\n", line);
            failed++;
            free_driver(driver);
        }
        free(line_copy);
    }

    fgets(line, 1000, rides_file); // skip first line
    while (fgets(line, 1000, rides_file) != NULL) {
        format_fgets_input_line(line);
        char *line_copy = g_strdup(line);
        Ride *ride = parse_line_ride(line_copy, ';');
        if (ride != NULL) {
            printf("Ride should've been NULL for line '%s'\n", line);
            failed++;
            free_ride(ride);
        }
        free(line_copy);
    }

    if (failed) {
        g_test_fail_printf("Failed for %d lines", failed);
    }
}

void assert_invalid_csv_loads_nothing_regular(void) {
    assert_invalid_csv_loads_nothing("datasets/data-regular-errors-invalid");
}

void assert_invalid_csv_loads_nothing_large(void) {
    assert_invalid_csv_loads_nothing("datasets/data-large-errors-invalid");
}

void assert_valid_csv_loads_everything_regular(void) {
    char *dataset_folder_path = "datasets/data-regular-errors-valid";

    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    char *line = malloc(sizeof(char) * 1000);

    int failed = 0;

    fgets(line, 1000, users_file); // skip first line
    while (fgets(line, 1000, users_file) != NULL) {
        format_fgets_input_line(line);
        char *line_copy = g_strdup(line);
        User *user = parse_line_user(line_copy, ';');
        if (user == NULL) {
            g_test_message("User should've been non-NULL for line '%s'\n", line);
            failed++;
        }
        free_user(user);
        free(line_copy);
    }

    fgets(line, 1000, drivers_file); // skip first line
    while (fgets(line, 1000, drivers_file) != NULL) {
        format_fgets_input_line(line);
        char *line_copy = g_strdup(line);
        Driver *driver = parse_line_driver(line_copy, ';');
        if (driver == NULL) {
            g_test_message("Driver should've been non-NULL for line '%s'\n", line);
            failed++;
        }
        free_driver(driver);
        free(line_copy);
    }

    fgets(line, 1000, rides_file); // skip first line
    while (fgets(line, 1000, rides_file) != NULL) {
        format_fgets_input_line(line);
        char *line_copy = g_strdup(line);
        Ride *ride = parse_line_ride(line_copy, ';');
        if (ride == NULL) {
            g_test_message("Ride should've been non-NULL for line '%s'\n", line);
            failed++;
        }
        free_ride(ride);
        free(line_copy);
    }

    if (failed) {
        g_test_fail_printf("Failed for %d lines", failed);
    }
}

int main(int argc, char *argv[]) {
    g_test_init(&argc, &argv, NULL);

    g_test_set_nonfatal_assertions();

#define ADD_TEST(testpath, function) g_test_add_func(testpath #function, function)

    ADD_TEST("/struct_utils/", assert_test_date_parse_and_encoding);
    ADD_TEST("/struct_utils/", assert_test_date_compare);
    ADD_TEST("/struct_utils/", assert_test_date_age);
    ADD_TEST("/parse_functions/", assert_invalid_csv_loads_nothing_large);
    ADD_TEST("/parse_functions/", assert_invalid_csv_loads_nothing_regular);
    ADD_TEST("/parse_functions/", assert_valid_csv_loads_everything_regular);
    ADD_TEST("/lazy/", test_lazy_behavior_int_apply_function);
    ADD_TEST("/lazy/", test_lazy_behavior_null_apply_function);
    ADD_TEST("/correctness/", load_catalog_execute_queries_and_check_expected_outputs_regular_1);
    ADD_TEST("/correctness/", load_catalog_execute_queries_and_check_expected_outputs_regular_2);
    ADD_TEST("/correctness/", load_catalog_execute_queries_and_check_expected_outputs_regular_1_lazy);
    ADD_TEST("/correctness/", load_catalog_execute_queries_and_check_expected_outputs_regular_2_lazy);
    ADD_TEST("/performance/", load_catalog_execute_queries_and_benchmark_regular_2);

    return g_test_run();
}
