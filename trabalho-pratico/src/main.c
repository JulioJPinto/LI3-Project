#include <stdio.h>


#include "catalog.h"
#include "file_util.h"
#include "logger.h"
#include "query_manager.h"
#include "parser.h"

int main(int argc, char **argv) {
    log_debug("Running on debug mode\n");

    if (argc < 2) {
        log_warning("Usage: %s <dataset_folder_path> <queries_file_path>", argv[0]);
        return 0;
    }

    GTimer *timer = g_timer_new();
    g_timer_start(timer);

    char *dataset_folder_path = argv[1];
    char *queries_file_path = argv[2];

    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    Catalog *catalog = create_catalog();

    char *line_buffer = malloc(1024 * sizeof(char));

    int count = 0;
    fgets(line_buffer, 1024, drivers_file);
    Driver *driver;
    while ((driver = read_drivers_file(drivers_file, line_buffer, 1024)) != NULL) {
        register_driver(catalog, driver);
        count++;
    }

    log_info("Read %d drivers.\n", count);

    count = 0;
    fgets(line_buffer, 1024, users_file);
    User *user;
    while ((user = read_users_file(users_file, line_buffer, 1024)) != NULL) {
        register_user(catalog, user);
        count++;
    }

    log_info("Read %d users.\n", count);

    count = 0;
    fgets(line_buffer, 1024, rides_file);
    Ride *ride;
    while ((ride = read_rides_file(rides_file, line_buffer, 1024)) != NULL) {
        register_ride(catalog, ride);
        count++;
    }

    log_info("Read %d rides.\n", count);

    fclose(users_file);
    fclose(drivers_file);
    fclose(rides_file);

    FILE *queries_file = open_file(queries_file_path);

    int query_count = 0;

    while (fgets(line_buffer, 1024, queries_file)) {
        format_fgets_input_line(line_buffer);
        log_info("Executing query '%s'\n", line_buffer);

        execute_query(catalog, stdout, line_buffer);
        query_count++;
    }

    free(line_buffer);
    fclose(queries_file);
    free_catalog(catalog);

    g_timer_stop(timer);

    log_info("Executed %d queries in %lf seconds.\n", query_count, g_timer_elapsed(timer, NULL));

    g_timer_destroy(timer);

    return 0;
}
