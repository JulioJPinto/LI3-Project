#include "query_manager.h"

#include <stdlib.h>

#include "queries.h"

static const QueryFunction query_functions[] = {
        execute_query_find_user_or_driver_by_name_or_id,
        execute_query_top_n_drivers,
        execute_query_longest_n_total_distance,
        execute_query_average_price_in_city,
        execute_query_average_price_in_date_range,
        execute_query_average_distance_in_city_in_date_range,
        execute_query_top_drivers_in_city_by_average_score,
        execute_query_rides_with_users_and_drivers_same_gender_by_account_creation_age,
        execute_query_passenger_that_gave_tip,
};

static const size_t query_functions_size = sizeof(query_functions) / sizeof(QueryFunction);

void parse_and_run_query(Catalog *catalog, FILE *output, char *query) {
    char **args = g_strsplit(query, " ", 0);

    int error = 0;
    int query_id = parse_int_safe(args[0], &error);

    if (error) {
        fprintf(output, "Invalid query id: %s\n", args[0]);
    } else {
        run_query(catalog, output, query_id, args + 1);
    }

    g_strfreev(args);
}

void run_query(Catalog *catalog, FILE *output, int query_id, char **args) {
    if (query_id <= 0 || query_id > (int) query_functions_size || query_functions[query_id - 1] == NULL) {
        fprintf(output, "Query %d not implemented\n", query_id);
        return;
    }

    query_functions[query_id - 1](catalog, output, args);
}
