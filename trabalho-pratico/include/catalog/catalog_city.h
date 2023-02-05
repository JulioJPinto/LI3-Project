#pragma once
#ifndef LI3_CATALOG_CITY_H
#define LI3_CATALOG_CITY_H

#include <glib.h>

/**
 * Struct that represents a catalog of cities.
 */
typedef struct CatalogCity CatalogCity;

/**
 * Creates a new empty CatalogCity allocated in the heap memory.
 */
CatalogCity *create_catalog_city(void);

/**
 * Frees the memory allocated for the catalog.
 */
void free_catalog_city(CatalogCity *catalog_city);

/**
 * Returns the city name associated with the given city id.
 * If the city is not registered, returns NULL.
 */
char *catalog_city_get_city_name(CatalogCity *catalog, int city_id);

/**
 * Returns the city id associated with the given city name.
 * If the city is not registered, returns -1.
 */
int catalog_city_get_city_id(CatalogCity *catalog, char *city);

/**
 * Returns the city id associated with the given city name.
 * If the city is not registered, it is registered and its id is returned.
 */
int catalog_city_get_or_register_city_id(CatalogCity *catalog, char *city);

#endif //LI3_CATALOG_CITY_H
