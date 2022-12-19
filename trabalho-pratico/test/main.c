#include "../include/catalog.h"
#include "../include/file_util.h"

#include <glib.h>
#include <stdio.h>

void assert_invalid_csv_loads_nothing(void) {
    char *dataset_folder_path = "datasets/data-regular-errors-invalid";

    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    char *line = malloc(sizeof(char) * 1000);

    fgets(line, 1000, users_file); // skip first line
    while (fgets(line, 1000, users_file) != NULL) {
        format_fgets_input_line(line);
        char *line_copy = g_strdup(line);
        User *user = parse_line_user(line_copy, ';');
        if (user != NULL) {
            g_test_fail_printf("User should've been NULL for line '%s'", line);
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
            g_test_fail_printf("Driver should've been NULL for line '%s'", line);
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
            g_test_fail_printf("Ride should've been NULL for line '%s'", line);
            free_ride(ride);
        }
        free(line_copy);
    }
}

int main(int argc, char *argv[]) {
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/assert_invalid_csv_loads_nothing", assert_invalid_csv_loads_nothing);

    return g_test_run();
}
