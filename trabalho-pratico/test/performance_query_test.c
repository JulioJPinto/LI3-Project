#include "catalog.h"

#define BENCHMARK_MAX_SECONDS_PER_QUERY 1

/**
 * Loads the catalog with the given dataset, executes the queries in the given file, and fails if any query takes longer than BENCHMARK_MAX_SECONDS_PER_QUERY seconds.
 */
void load_catalog_execute_queries_and_benchmark(char *dataset_folder_path, char *queries_file_path) {
    Catalog *catalog = create_catalog();
    catalog_load_dataset(catalog, dataset_folder_path);

    // Don't force eager indexing, so we get worse case scenario

    FILE *queries_file = open_file(queries_file_path);

    OutputWriter *output_writer = create_null_output_writer();

    char buffer[1024];
    while (fgets(buffer, 1024, queries_file) != NULL) {
        format_fgets_input_line(buffer);

        g_autofree GTimer *timer = g_timer_new();
        g_timer_start(timer);
        parse_and_run_query(catalog, output_writer, buffer);
        g_timer_stop(timer);

        if (g_timer_elapsed(timer, NULL) > BENCHMARK_MAX_SECONDS_PER_QUERY) {
            fprintf(stderr, "Query '%s' too long: %f seconds\n", buffer, g_timer_elapsed(timer, NULL));
            g_test_fail();
        }
    }

    close_output_writer(output_writer);
    fclose(queries_file);
    free_catalog(catalog);
}

/**
 * Ensures that no query in `datasets/data-regular/input2.txt` takes longer than BENCHMARK_MAX_SECONDS_PER_QUERY seconds.
 */
void load_catalog_execute_queries_and_benchmark_regular_2(void) {
    load_catalog_execute_queries_and_benchmark("datasets/data-regular", "datasets/data-regular/input2.txt");
}
