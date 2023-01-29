#pragma once
#ifndef LI3_CATALOG_DRIVER_CITY_INFO_H
#define LI3_CATALOG_DRIVER_CITY_INFO_H

#include <glib.h>

#include "driver_city_info.h"

/**
 * Struct that represents a catalog of driver city info.
 * The catalog is needed because having an array (each element being information about the driver in a city) for each driver would be too expensive.
 */
typedef struct CatalogDriverCityInfo CatalogDriverCityInfo;

/**
 * Creates a new empty CatalogDriverCityInfo allocated in the heap memory.
 */
CatalogDriverCityInfo *create_catalog_driver_city_info(void);

/**
 * Frees the memory allocated for the catalog.
 */
void free_catalog_driver_city_info(CatalogDriverCityInfo *catalog_driver_city_info);

/**
 * Registers the driver score from a ride in a city in the catalog.
 */
void catalog_driver_city_info_register(CatalogDriverCityInfo *catalog, int driver_id, int driver_score, int ride_city_id);

/**
 * Forces the catalog to index all the driver city info.
 */
void catalog_driver_city_info_force_eager_indexing(CatalogDriverCityInfo *catalog);

/**
 * Retrieves the top n driver city info with the best score in the given city (using `compare_driver_city_infos_by_average_score`).
 * The result is stored in the given GPtrArray.
 */
int catalog_driver_city_info_get_top_best_drivers_by_city(CatalogDriverCityInfo *catalog, int city_id, int n, GPtrArray *result);

#endif //LI3_CATALOG_DRIVER_CITY_INFO_H
