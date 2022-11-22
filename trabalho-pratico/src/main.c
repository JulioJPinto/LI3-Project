#include <stdio.h>

#include "catalog.h"
#include "file_util.h"
#include "logger.h"
#include "query_manager.h"
#include "parser.h"
#include "main_wrappers.h"

#define OUTPUT_FOLDER "Resultados"

/**
 * Main entry point
 */
int main(int argc, char **argv) {
    log_debug("Running on debug mode\n");

    if (argc < 2) {
        log_warning("Usage: %s <dataset_folder_path> <queries_file_path>\n", argv[0]);
        return 0;
    }

    GTimer *global_timer = g_timer_new();
    g_timer_start(global_timer);

    GTimer *loading_timer = g_timer_new();
    g_timer_start(loading_timer);

    char *dataset_folder_path = argv[1];
    char *queries_file_path = argv[2];

    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    Catalog *catalog = create_catalog();

    char *line_buffer = malloc(1024 * sizeof(char));

    read_file(users_file, wrapper_voidp_parse_user, wrapper_voidp_register_user, catalog);
    read_file(drivers_file, wrapper_voidp_parse_driver, wrapper_voidp_register_driver, catalog);
    read_file(rides_file, wrapper_voidp_parse_ride, wrapper_voidp_register_ride, catalog);

    notify_stop_registering(catalog);

    g_timer_stop(loading_timer);

    GTimer *total_query_time_timer = g_timer_new();
    g_timer_start(total_query_time_timer);

    FILE *queries_file = open_file(queries_file_path);

    int query_count = 0;

    create_output_folder_if_not_exists();

    GTimer *query_execution_timer = g_timer_new();
    while (fgets(line_buffer, 1024, queries_file)) {
        g_timer_start(query_execution_timer);
        format_fgets_input_line(line_buffer);
        log_info("Executing query '%s'", line_buffer);

        FILE *output_file = create_command_output_file(query_count + 1);

        parse_and_run_query(catalog, output_file, line_buffer);
        fclose(output_file);

        log_info(" (%.3fms)\n", g_timer_elapsed(query_execution_timer, NULL) * 1000);
        query_count++;
    }

    g_timer_stop(total_query_time_timer);

    fclose(users_file);
    fclose(drivers_file);
    fclose(rides_file);

    free(line_buffer);
    fclose(queries_file);
    free_catalog(catalog);

    g_timer_stop(global_timer);

    log_info("Loading time:                   %lfs\n", g_timer_elapsed(loading_timer, NULL));
    log_info("Execution time (%d queries):    %lfs\n", query_count, g_timer_elapsed(total_query_time_timer, NULL));
    log_info("Total runtime:                  %lfs\n", g_timer_elapsed(global_timer, NULL));

    g_timer_destroy(query_execution_timer);
    g_timer_destroy(loading_timer);
    g_timer_destroy(total_query_time_timer);
    g_timer_destroy(global_timer);

    return 0;
}
