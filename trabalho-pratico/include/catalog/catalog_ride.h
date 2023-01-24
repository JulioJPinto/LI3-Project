#pragma once
#ifndef LI3_CATALOG_RIDE_H
#define LI3_CATALOG_RIDE_H

#include <glib.h>

#include "ride.h"

typedef struct CatalogRide CatalogRide;

CatalogRide *create_catalog_ride(void);

void free_catalog_ride(CatalogRide *catalog_ride);

void catalog_ride_register_ride(CatalogRide *catalog_ride, Ride *ride);

void catalog_ride_register_ride_same_gender(CatalogRide *catalog_ride, Gender gender, Ride *ride);

Ride *catalog_ride_get_ride(CatalogRide *catalog_ride, int ride_id);

gboolean catalog_ride_city_has_rides(CatalogRide *catalog_ride, char *city);

double catalog_ride_get_average_price_in_city(CatalogRide *catalog_ride, char *city);

double catalog_ride_get_average_distance_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date);

double catalog_ride_get_average_distance_in_city_and_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, char *city);

void catalog_ride_get_passengers_that_gave_tip_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, GPtrArray *result);

int catalog_ride_get_rides_with_user_and_driver_with_same_age_above_acc_age(CatalogRide *catalog_ride, GPtrArray *result, Gender gender, int min_account_age);

void catalog_ride_force_eager_indexing(CatalogRide *catalog_ride);

#endif //LI3_CATALOG_RIDE_H
