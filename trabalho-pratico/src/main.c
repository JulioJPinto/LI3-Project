#include "benchmark.h"
#include "program.h"

/**
 * Main entry point
 */
int main(int argc, char **argv) {
    log_debug("Running on debug mode\n");

    BENCHMARK_START(global_timer);
    Program *program = create_program();

    int result = run_program(program, argv, argc);

    free_program(program);

    gboolean lazy_loading = FALSE;

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
    if (!lazy_loading) notify_stop_registering(catalog);
    BENCHMARK_END(load_timer, "Final indexing time:    %f seconds\n");

    g_timer_stop(loading_timer);

    BENCHMARK_START(total_query_time_timer);

    FILE *queries_file = open_file(queries_file_path);

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
