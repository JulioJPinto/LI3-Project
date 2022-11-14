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

User *get_user_from_username(Catalog *catalog, char* username);

Driver *get_driver_from_id(Catalog *catalog, int id);

double get_price_average_from_city(Catalog *catalog, GString city_name);

#endif //LI3_CATALOG_H
