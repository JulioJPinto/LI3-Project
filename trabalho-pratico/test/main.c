#include "../include/catalog.h"
#include "../include/file_util.h"

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

void test_date(char *date, int expected_day, int expected_month, int expected_year) {
    char *date_copy = g_strdup(date);
    Date parsed_date = parse_date(date_copy);
    if (date_get_day(parsed_date) != expected_day) {
        g_test_fail_printf("Day should've been %d for date '%s' but is %d\n", expected_day, date, date_get_day(parsed_date));
    }
    if (date_get_month(parsed_date) != expected_month) {
        g_test_fail_printf("Month should've been %d for date '%s' but is %d\n", expected_month, date, date_get_month(parsed_date));
    }
    if (date_get_year(parsed_date) != expected_year) {
        g_test_fail_printf("Year should've been %d for date '%s' but is %d\n", expected_year, date, date_get_year(parsed_date));
    }
    free(date_copy);
}

void test_date_parse_and_encoding(void) {
    test_date("01/01/2022", 1, 1, 2022);
    test_date("31/12/2000", 31, 12, 2000);

    char date3[] = "38/01/2001";
    if (is_date_valid(parse_date(date3))) {
        g_test_fail_printf("Date should've been invalid for date '%s'\n", date3);
    }
}

void test_date_compare(void) {
    Date date1 = create_date(1, 1, 2000);
    Date date3 = create_date(1, 1, 2001);
    Date date4 = create_date(1, 2, 2000);
    Date date5 = create_date(2, 1, 2000);
    Date date6 = create_date(8, 5, 1999);

    if (date_compare(date1, date1) != 0) {
        g_test_fail_printf("Date1 (01/01/2000) should've been equal to itself");
    }
    if (date_compare(date1, date3) >= 0) {
        g_test_fail_printf("Date1 (01/01/2000) should've been before Date3 (01/01/2001)");
    }
    if (date_compare(date1, date4) >= 0) {
        g_test_fail_printf("Date1 (01/01/2000) should've been before Date4 (01/02/2000)");
    }
    if (date_compare(date5, date1) <= 0) {
        g_test_fail_printf("Date5 (02/01/2000) should've been after Date1 (01/01/2000)");
    }
    if (date_compare(date6, date1) >= 0) {
        g_test_fail_printf("Date6 (08/05/1999) should've been before Date1 (01/01/2000)");
    }
}


// 9/10/2022
void test_date_age(void) {
    Date date1 = create_date(1, 1, 2000);
    Date date2 = create_date(7, 2, 2005);
    Date date3 = create_date(9, 10, 2022);
    Date date4 = create_date(9, 10, 2021);

    if (get_age(date1) != 22) {
        g_test_fail_printf("Age should've been 22 for date1 (01/01/2000) but is %d", get_age(date1));
    }
    if (get_age(date2) != 17) {
        g_test_fail_printf("Age should've been 17 for date2 (07/02/2005) but is %d", get_age(date2));
    }
    if (get_age(date3) != 0) {
        g_test_fail_printf("Age should've been 0 for date3 (09/10/2022) but is %d", get_age(date3));
    }
    if (get_age(date4) != 1) {
        g_test_fail_printf("Age should've been 1 for date4 (09/10/2021) but is %d", get_age(date4));
    }
}

int main(int argc, char *argv[]) {
    g_test_init(&argc, &argv, NULL);

    g_test_set_nonfatal_assertions();

    g_test_add_func("/assert_date_correct_parse_and_encoding", test_date_parse_and_encoding);
    g_test_add_func("/assert_date_correct_compare", test_date_compare);
    g_test_add_func("/assert_date_correct_age", test_date_age);
    g_test_add_func("/assert_invalid_csv_loads_nothing_large", assert_invalid_csv_loads_nothing_large);
    g_test_add_func("/assert_invalid_csv_loads_nothing_regular", assert_invalid_csv_loads_nothing_regular);
    g_test_add_func("/assert_valid_csv_loads_everything", assert_valid_csv_loads_everything_regular);

    return g_test_run();
}
