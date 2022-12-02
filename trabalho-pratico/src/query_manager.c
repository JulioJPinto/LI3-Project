#include "query_manager.h"

#include <stdlib.h>

#include "queries.h"

static const QueryFunction query_functions[] = {
        execute_query_find_user_or_driver_by_name_or_id,
        execute_query_top_n_drivers,
        execute_query_longest_n_total_distance,
        execute_query_average_price_in_city,
        execute_query_average_price_in_date_range,
        execute_average_distance_per_city_per_date,
};

static const size_t query_functions_size = sizeof(query_functions) / sizeof(QueryFunction);

void parse_and_run_query(Catalog *catalog, FILE *output, char *query) {
    char **args = g_strsplit(query, " ", 0);

    char *query_id_str_left;
    int query_id = (int) strtol(args[0], &query_id_str_left, 10);

    if (*query_id_str_left != '\0') {
        fprintf(output, "Invalid query id: %s\n", args[0]);
    } else {
        run_query(catalog, output, query_id, args + 1);
    }

    g_strfreev(args);
}

void run_query(Catalog *catalog, FILE *output, int query_id, char **args) {
    if (query_id <= 0 || query_id > (int) query_functions_size) {
        fprintf(output, "Query %d not implemented\n", query_id);
        return;
    }
    query_functions[query_id - 1](catalog, output, args);
}
