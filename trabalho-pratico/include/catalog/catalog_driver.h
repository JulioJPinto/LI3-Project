#pragma once
#ifndef LI3_CATALOG_DRIVER_H
#define LI3_CATALOG_DRIVER_H

#include <glib.h>
#include "driver.h"

/**
 * Struct that represents a catalog of drivers.
 */
typedef struct CatalogDriver CatalogDriver;

/**
 * Creates a new empty CatalogDriver allocated in the heap memory.
 */
CatalogDriver *create_catalog_driver(void);

/**
 * Frees the memory allocated for the catalog.
 */
void free_catalog_driver(CatalogDriver *catalog_driver);

/**
 * Registers a driver in the catalog.
 */
void catalog_driver_register_driver(CatalogDriver *catalog_driver, Driver *driver);

/**
 * Registers ride information from a driver in the catalog.
 * This function calls CatalogDriverCityInfo to register the ride information in the driver city info.
 */
void catalog_driver_register_driver_ride(CatalogDriver *catalog_driver, Driver *driver, int driver_score, int city_id);

/**
 * Returns the driver with the given id.
 */
Driver *catalog_driver_get_driver(CatalogDriver *catalog_driver, int driver_id);

/**
 * Retrieves the top n drivers with the best score (using `compare_drivers_by_score`).
 * The result is stored in the given GPtrArray.
 */
int catalog_driver_get_top_n_drivers_with_best_score(CatalogDriver *catalog_driver, int n, GPtrArray *result);

/**
 * Retrieves the top n drivers with the best score in the given city (using `compare_drivers_by_score`).
 * The result is stored in the given GPtrArray.
 */
int catalog_driver_get_top_n_drivers_with_best_score_by_city(CatalogDriver *catalog_driver, int city_id, int n, GPtrArray *result);

/**
 * Forces the catalog to index all the drivers.
 */
void catalog_driver_force_eager_indexing(CatalogDriver *catalog_driver);

#endif //LI3_CATALOG_DRIVER_H
