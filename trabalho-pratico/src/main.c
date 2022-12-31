#include <stdio.h>

#include "catalog.h"
#include "file_util.h"
#include "logger.h"
#include "query_manager.h"
#include "parser.h"

#define OUTPUT_FOLDER "Resultados"

#define BENCHMARK_START(timer_name)                \
    g_autofree GTimer *timer_name = g_timer_new(); \
    g_timer_start(timer_name)

#define BENCHMARK_END(timer_name, log_string) \
    g_timer_stop(timer_name);                 \
    log_info(log_string, g_timer_elapsed(timer_name, NULL))

/**
 * Main entry point
 */
int main(int argc, char **argv) {
    log_debug("Running on debug mode\n");

    if (argc < 2) {
        log_warning("Usage: %s <dataset_folder_path> <queries_file_path>\n", argv[0]);
        return 0;
    }

    BENCHMARK_START(global_timer);
    BENCHMARK_START(loading_timer);

    char *dataset_folder_path = argv[1];
    char *queries_file_path = argv[2];

    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    FILE *queries_file = open_file(queries_file_path);

    Catalog *catalog = create_catalog();

    char *line_buffer = malloc(1024 * sizeof(char));

    BENCHMARK_START(load_timer);
    read_file(users_file, parse_and_register_user, catalog);
    BENCHMARK_END(load_timer, "Load users time:        %f seconds\n");

    g_timer_start(load_timer);
    read_file(drivers_file, parse_and_register_driver, catalog);
    BENCHMARK_END(load_timer, "Load drivers time:      %f seconds\n");

    g_timer_start(load_timer);
    read_file(rides_file, parse_and_register_ride, catalog);
    BENCHMARK_END(load_timer, "Load rides time:        %f seconds\n");

    g_timer_start(load_timer);
    notify_stop_registering(catalog);
    BENCHMARK_END(load_timer, "Final indexing time:    %f seconds\n");

    g_timer_stop(loading_timer);

    BENCHMARK_START(total_query_time_timer);

    int query_count = 0;

    create_output_folder_if_not_exists();

    BENCHMARK_START(query_execution_timer);
    while (fgets(line_buffer, 1024, queries_file)) {
        g_timer_start(query_execution_timer);
        format_fgets_input_line(line_buffer);

        query_count++;
        log_info("Executing query (%d) '%s'", query_count, line_buffer);

        if (*line_buffer == '#') {
            log_info(" (skipped)\n");
            continue;
        }

        FILE *output_file = create_command_output_file(query_count);

        parse_and_run_query(catalog, output_file, line_buffer);
        fclose(output_file);

        log_info(" (%.3fms)\n", g_timer_elapsed(query_execution_timer, NULL) * 1000);
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

    return 0;
}
