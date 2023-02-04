#include "struct_util_test.c"
#include "lazy_test.c"
#include "correctness_parser_test.c"
#include "correctness_query_test.c"
#include "performance_query_test.c"
#include "output_writer_test.c"
#include "token_iterator_test.c"

#include <glib.h>

/**
 * Entry point for the test suite.
*/
int main(int argc, char *argv[]) {
    g_test_init(&argc, &argv, NULL);

    g_test_set_nonfatal_assertions();

/**
 * Adds a test to the test suite with the testpath concatenated with the function name.
*/
#define ADD_TEST(testpath, function) g_test_add_func(testpath #function, function)

    ADD_TEST("/struct_utils/", assert_test_date_parse_and_encoding);
    ADD_TEST("/struct_utils/", assert_test_date_compare);
    ADD_TEST("/struct_utils/", assert_test_date_age);
    ADD_TEST("/lazy/", test_lazy_behavior_int_apply_function);
    ADD_TEST("/lazy/", test_lazy_behavior_null_apply_function);
    ADD_TEST("/token_iterator/", test_semicolon_separated_token_iterator);
    ADD_TEST("/output_writer/", test_semicolon_file_output_writer);
    ADD_TEST("/output_writer/", test_array_of_semicolon_strings_output_writer);
    ADD_TEST("/correctness/parser/", assert_invalid_csv_loads_nothing_large);
    ADD_TEST("/correctness/parser/", assert_invalid_csv_loads_nothing_regular);
    ADD_TEST("/correctness/parser/", assert_valid_csv_loads_everything_regular);
    ADD_TEST("/correctness/query/", load_catalog_execute_queries_and_check_expected_outputs_regular_1);
    ADD_TEST("/correctness/query/", load_catalog_execute_queries_and_check_expected_outputs_regular_2);
    ADD_TEST("/correctness/query/", load_catalog_execute_queries_and_check_expected_outputs_regular_1_lazy);
    ADD_TEST("/correctness/query/", load_catalog_execute_queries_and_check_expected_outputs_regular_2_lazy);
    ADD_TEST("/performance/", load_catalog_execute_queries_and_benchmark_regular_2);

    return g_test_run();
}
