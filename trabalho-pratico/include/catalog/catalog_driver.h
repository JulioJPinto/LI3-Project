#pragma once
#ifndef LI3_CATALOG_DRIVER_H
#define LI3_CATALOG_DRIVER_H

#include <glib.h>
#include "driver.h"

typedef struct CatalogDriver CatalogDriver;

CatalogDriver *create_catalog_driver(void);

void free_catalog_driver(CatalogDriver *catalog_driver);

void catalog_driver_register_driver(CatalogDriver *catalog_driver, Driver *driver);

Driver *catalog_driver_get_driver(CatalogDriver *catalog_driver, int driver_id);

int catalog_driver_get_top_n_drivers_with_best_score(CatalogDriver *catalog_driver, int n, GPtrArray *result);

void catalog_driver_notify_stop_registering(CatalogDriver *catalog_driver);

#endif //LI3_CATALOG_DRIVER_H
