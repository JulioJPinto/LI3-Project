#pragma once
#ifndef LI3_CATALOG_DRIVER_CITY_INFO_H
#define LI3_CATALOG_DRIVER_CITY_INFO_H

#include <glib.h>

#include "driver_city_info.h"

typedef struct CatalogDriverCityInfo CatalogDriverCityInfo;

CatalogDriverCityInfo *create_catalog_driver_city_info(void);

void free_catalog_driver_city_info(CatalogDriverCityInfo *catalog_driver_city_info);

void catalog_driver_city_info_register(CatalogDriverCityInfo *catalog, int driver_id, int driver_score, char *ride_city);

void catalog_driver_city_info_force_eager_indexing(CatalogDriverCityInfo *catalog);

int catalog_driver_city_info_get_top_best_drivers_by_city(CatalogDriverCityInfo *catalog, char *city, int n, GPtrArray *result);

#endif //LI3_CATALOG_DRIVER_CITY_INFO_H
