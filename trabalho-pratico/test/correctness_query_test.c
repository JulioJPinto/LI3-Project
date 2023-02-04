#include "catalog.h"
#include "catalog_loader.h"
#include "query_manager.h"

/**
 * Gets the index-th element of the array. If the index is out of bounds, the default value is returned.
 */
#define GET_INDEX_SAFE(array, index, default_value) (index < (int) array->len ? array->pdata[index] : default_value)

/**
 * Loads the catalog from the given dataset folder path.
 * If lazy_loading is TRUE, the catalog is indexed lazily.
 * Executes the queries from the given queries file path.
 * Loads the expected output of the queries from the given expected query result folder path.
 * Checks if the output of the queries is the same as the expected output.
 * If not the same, prints the query, the actual and the expected output.
 */
void load_catalog_execute_queries_and_check_expected_outputs(char *dataset_folder_path,
                                                             char *queries_file_path,
                                                             char *expected_query_result_folder_path,
                                                             gboolean lazy_loading) {
    Catalog *catalog = create_catalog();
    catalog_load_csv_dataset(catalog, dataset_folder_path);

    if (!lazy_loading) catalog_force_eager_indexing(catalog);

    FILE *queries_file = open_file(queries_file_path);

    int current_query_id = 1;

    char buffer[1000];
    while (fgets(buffer, 1000, queries_file) != NULL) {
        format_input_line(buffer);

        char *query = g_strdup(buffer);

        GPtrArray *actualResult = g_ptr_array_new_with_free_func(free);
        GPtrArray *expectedResult = g_ptr_array_new_with_free_func(free);

        OutputWriter *writer = create_array_of_semicolon_strings_output_writer(actualResult);
        OutputWriter *expectedWriter = create_array_of_semicolon_strings_output_writer(expectedResult);

        parse_and_run_query(catalog, writer, query);

        char *expected_query_result_file_path = g_strdup_printf("command%d_output.txt", current_query_id);
        FILE *expected_query_result_file = open_file_folder(expected_query_result_folder_path, expected_query_result_file_path);

        while (fgets(buffer, 1000, expected_query_result_file) != NULL) {
            format_input_line(buffer);
            writer_write_output_token_end(expectedWriter, buffer);
        }


        int maxFileLines = MAX(actualResult->len, expectedResult->len);

        for (int i = 0; i < maxFileLines; i++) {
            char *expectedLine = GET_INDEX_SAFE(expectedResult, i, "");
            char *actualLine = GET_INDEX_SAFE(actualResult, i, "");
            if (strcmp(expectedLine, actualLine) != 0) {
                g_test_fail();

                format_input_line(expectedLine);
                format_input_line(actualLine);

                fprintf(stderr, "Query %d (%s) failed:\n", current_query_id, query);
                fprintf(stderr, "Expected: '%s'\n", expectedLine);
                fprintf(stderr, "Actual:   '%s'\n\n", actualLine);
            }
        }
        current_query_id++;

        close_output_writer(writer);
        close_output_writer(expectedWriter);
        g_ptr_array_free(actualResult, TRUE);
        g_ptr_array_free(expectedResult, TRUE);
        free(query);
        free(expected_query_result_file_path);
        fclose(expected_query_result_file);
    }

    fclose(queries_file);
    free_catalog(catalog);
}

/**
 * Checks if all the queries from `data-regular/input1.txt` return the expected output with eager indexing.
 */
void load_catalog_execute_queries_and_check_expected_outputs_regular_1(void) {
    load_catalog_execute_queries_and_check_expected_outputs("datasets/data-regular",
                                                            "datasets/data-regular/input1.txt",
                                                            "datasets/data-regular/expected-results-1",
                                                            FALSE);
}

/**
 * Checks if all the queries from `data-regular/input2.txt` return the expected output with eager indexing.
 */
void load_catalog_execute_queries_and_check_expected_outputs_regular_2(void) {
    load_catalog_execute_queries_and_check_expected_outputs("datasets/data-regular",
                                                            "datasets/data-regular/input2.txt",
                                                            "datasets/data-regular/expected-results-2",
                                                            FALSE);
}

/**
 * Checks if all the queries from `data-regular/input1.txt` return the expected output with lazy indexing.
 */
void load_catalog_execute_queries_and_check_expected_outputs_regular_1_lazy(void) {
    load_catalog_execute_queries_and_check_expected_outputs("datasets/data-regular",
                                                            "datasets/data-regular/input1.txt",
                                                            "datasets/data-regular/expected-results-1",
                                                            TRUE);
}

/**
 * Checks if all the queries from `data-regular/input2.txt` return the expected output with lazy indexing.
 */
void load_catalog_execute_queries_and_check_expected_outputs_regular_2_lazy(void) {
    load_catalog_execute_queries_and_check_expected_outputs("datasets/data-regular",
                                                            "datasets/data-regular/input2.txt",
                                                            "datasets/data-regular/expected-results-2",
                                                            TRUE);
}
