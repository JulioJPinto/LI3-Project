#include "query_manager.h"

#include "queries.h"
#include "logger.h"

/**
 * Struct that holds information about a query command.
 */
typedef struct {
    QueryFunction *function;
    int min_args;
    char *usage;
    char *description;
} QueryFunctionInfo;

/**
 * Array that holds every query command.
 */
static const QueryFunctionInfo query_functions[] = {
        {execute_query_find_user_or_driver_by_name_or_id, 1, "1 <username|id>", "Finds a user/driver by its name/ID"},
        {execute_query_top_n_drivers, 1, "2 <n>", "Gets the n drivers with the best score"},
        {execute_query_longest_n_total_distance, 1, "3 <n>", "Gets the n users with the longest accumulated distance"},
        {execute_query_average_price_in_city, 1, "4 <city>", "Gets the average price of a ride for a specific city"},
        {execute_query_average_price_in_date_range, 2, "5 <start_date> <end_date>", "Gets the average price of a ride in a given time span"},
        {execute_query_average_distance_in_city_in_date_range, 3, "6 <city> <start_date> <end_date>", "Gets the average distance for a ride in a give time span for a specific city"},
        {execute_query_top_drivers_in_city_by_average_score, 2, "7 <n> <city>", "Gets the n drivers with the best score in a specific city"},
        {execute_query_rides_with_users_and_drivers_same_gender_by_account_creation_age, 2, "8 <gender> <min_account_age>", "Gets the rides where the user and drivers have the same gender by account creation age"},
        {execute_query_passenger_that_gave_tip, 2, "9 <start_date> <end_date>",  "Gets the users who gave a tip in a certain time span"},
};

/**
 * Size of the query_functions array.
 */
static const size_t query_functions_size = sizeof(query_functions) / sizeof(QueryFunctionInfo);

void parse_and_run_query(Catalog *catalog, OutputWriter *output, char *query) {
    char **args = g_strsplit(query, " ", 0);

    int error = 0;
    int query_id = parse_int_safe(args[0], &error);

    if (error) {
        writer_write_output_token_end(output, "Invalid query id: %s\n", args[0]);
    } else {
        run_query(catalog, output, query_id, args + 1);
    }

    g_strfreev(args);
}

void run_query(Catalog *catalog, OutputWriter *output, int query_id, char **args) {
    if (query_id <= 0 || query_id > (int) query_functions_size) {
        writer_write_output_token_end(output, "Query %d not implemented\n", query_id);
        return;
    }

    QueryFunctionInfo query_function_info = query_functions[query_id - 1];
    int min_args = query_function_info.min_args;
    int args_count = (int) g_strv_length(args);

    if (args_count < min_args) {
        fprintf(stdout, TERMINAL_YELLOW_BOLD "Query %d " TERMINAL_RESET "- %s\n", query_id, query_function_info.description);
        LOG_WARNING_VA("Usage: '%s'", query_function_info.usage);
        return;
    }

    query_function_info.function(catalog, output, args);
}
