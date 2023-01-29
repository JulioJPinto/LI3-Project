#pragma once
#ifndef LI3_CATALOG_RIDE_H
#define LI3_CATALOG_RIDE_H

#include <glib.h>

#include "ride.h"

/**
 * Struct that represents a catalog of rides.
 */
typedef struct CatalogRide CatalogRide;

/**
 * Creates a new empty CatalogRide allocated in the heap memory.
 */
CatalogRide *create_catalog_ride(void);

/**
 * Frees the memory allocated for the catalog.
 */
void free_catalog_ride(CatalogRide *catalog_ride);

/**
 * Registers a ride in the catalog.
 */
void catalog_ride_register_ride(CatalogRide *catalog_ride, Ride *ride);

/**
 * Registers a ride in the catalog whose driver and user have the same gender.
 */
void catalog_ride_register_ride_same_gender(CatalogRide *catalog_ride, Gender gender, Ride *ride);

/**
 * Returns the average price of rides in the given city.
 */
double catalog_ride_get_average_price_in_city(CatalogRide *catalog_ride, int city_id);

/**
 * Returns the average distance in the given date range.
 */
double catalog_ride_get_average_distance_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date);

/**
 * Returns the average distance in the given city and date range.
 */
double catalog_ride_get_average_distance_in_city_and_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, int city_id);

/**
 * Retrieves the rides in the given date range whose passenger gave a tip.
 * The result is stored in the given GPtrArray.
 */
void catalog_ride_get_passengers_that_gave_tip_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, GPtrArray *result);

/**
 * Retrieves the rides whose user and driver have the same gender and both have an account age above the given age.
 */
int catalog_ride_get_rides_with_user_and_driver_with_same_age_above_acc_age(CatalogRide *catalog_ride, GPtrArray *result, Gender gender, int min_account_age);

/**
 * Forces the catalog to index all the rides.
 */
void catalog_ride_force_eager_indexing(CatalogRide *catalog_ride);

#endif //LI3_CATALOG_RIDE_H
