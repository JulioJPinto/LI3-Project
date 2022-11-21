#pragma once
#ifndef LI3_QUERY_MANAGER_H
#define LI3_QUERY_MANAGER_H

#include <stdio.h>

#include "catalog.h"

/**
 * Parses a raw string format and calls `run_query`.
 * It is safe to call this function with bad input.
 */
void parse_and_run_query(Catalog *catalog, FILE *output, char *query);

/**
 * Runs the query with the given id and args in catalog and writes the result to the output stream.
 */
void run_query(Catalog *catalog, FILE *output, int query_id, char **args);

#endif //LI3_QUERY_MANAGER_H
