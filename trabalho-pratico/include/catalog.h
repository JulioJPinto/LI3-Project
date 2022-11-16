#pragma once
#ifndef LI3_CATALOG_H
#define LI3_CATALOG_H

#include <glib.h>

#include "driver.h"
#include "ride.h"
#include "user.h"

typedef struct Catalog Catalog;

Catalog *create_catalog(void);

void free_catalog(Catalog *catalog);

void register_user(Catalog *catalog, User *user);

void register_driver(Catalog *catalog, Driver *driver);

void register_ride(Catalog *catalog, Ride *ride);

void notify_stop_registering(Catalog *catalog);

User *catalog_get_user(Catalog *catalog, char *username);

Driver *catalog_get_driver(Catalog *catalog, int id);

/*
 * Inserts the top N drivers in the given GPtrArray.
 * Drivers are sorted by their score, date of last ride and id.
 * The drivers are not copied, their pointers are inserted.
 * The GPtrArray should be empty and pre-allocated with N elements.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
void catalog_get_top_n_drivers(Catalog *catalog, int n, GPtrArray *result);

void catalog_get_longest_n_total_distance(Catalog *catalog, int n, GPtrArray *result);

double get_price_average_from_city(Catalog *catalog, GString city_name);

#endif //LI3_CATALOG_H
