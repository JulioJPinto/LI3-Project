#pragma once
#ifndef LI3_CATALOG_RIDE_H
#define LI3_CATALOG_RIDE_H

#include <glib.h>

#include "ride.h"

typedef struct CatalogRide CatalogRide;

CatalogRide *create_catalog_ride(void);

void free_catalog_ride(CatalogRide *catalog_ride);

void catalog_ride_register_ride(CatalogRide *catalog_ride, Ride *ride);

gboolean catalog_ride_city_has_rides(CatalogRide *catalog_ride, char *city);

double catalog_ride_get_average_price_in_city(CatalogRide *catalog_ride, char *city);

double catalog_ride_get_average_distance_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date);

double catalog_ride_get_average_distance_in_city_and_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, char *city);

void catalog_ride_get_passengers_that_gave_tip_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, GPtrArray *result);

void catalog_ride_notify_stop_registering(CatalogRide *catalog_ride);

#endif //LI3_CATALOG_RIDE_H
