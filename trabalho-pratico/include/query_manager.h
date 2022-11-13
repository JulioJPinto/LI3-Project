#pragma once
#ifndef LI3_QUERY_MANAGER_H
#define LI3_QUERY_MANAGER_H

#include <glib.h>
#include <stdio.h>

#include "catalog.h"

void execute_query(Catalog *catalog, FILE *output, char *query);

void run_query(Catalog *catalog, FILE *output, int query_id, char **args);

#endif //LI3_QUERY_MANAGER_H
