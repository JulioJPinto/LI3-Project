#pragma once
#ifndef LI3_QUERIES_H
#define LI3_QUERIES_H

#include <stdio.h>
#include "catalog.h"

typedef void (*QueryFunction)(Catalog *, FILE *, char **);

/*
 * Executes the query number 1.
 */
void execute_query_find_user_or_driver_by_name_or_id(Catalog *catalog, FILE *output, char **args);

/*
 * Executes the query number 2.
 */
void execute_query_top_n_drivers(Catalog *catalog, FILE *output, char **args);

#endif //LI3_QUERIES_H
