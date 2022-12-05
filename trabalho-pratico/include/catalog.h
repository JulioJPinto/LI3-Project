#pragma once
#ifndef LI3_CATALOG_H
#define LI3_CATALOG_H

#include <glib.h>

#include "driver.h"
#include "ride.h"
#include "user.h"

typedef struct DriverbyCity DriverbyCity;

DriverbyCity *create_driver_by_city(int id, char* name);

int driver_by_city_get_id(DriverbyCity *driver);

char *driver_by_city_get_name(DriverbyCity *driver);

void driver_by_city_increment_number_of_rides(DriverbyCity *driver);

void driver_by_city_add_score(DriverbyCity *driver, int score);

double driver_by_city_get_average_score(DriverbyCity *driver);

/**
 * Struct that represents a catalog.
 */
typedef struct Catalog Catalog;

/**
 * Creates a new empty Catalog allocated in the heap memory.
 */
Catalog *create_catalog(void);

/**
 * Frees the memory allocated for the catalog.
 */
void free_catalog(Catalog *catalog);

/**
 * Registers a user in the catalog.
 */
void register_user(Catalog *catalog, User *user);

/**
 * Registers a driver in the catalog.
 */
void register_driver(Catalog *catalog, Driver *driver);

/**
 * Registers a ride in the catalog.
 * The user and driver linked to the ride must be already registered in the catalog
 * as it updates the user and driver information related to this ride.
 */
void register_ride(Catalog *catalog, Ride *ride);

/**
 * Notifies the catalog that the program won't register any more data.
 * This will allow the catalog to optimize its internal data structures.
 * This function must be called before any query.
 */
void notify_stop_registering(Catalog *catalog);

/**
 * Returns a pointer to the user already registered with the given username.
 * Returns NULL if no user with the given username exists.
 * The returned pointer is owned by the catalog and must not be freed.
 */
User *catalog_get_user(Catalog *catalog, char *username);

/**
 * Returns a pointer to the driver already registered with the given id.
 * Returns NULL if no driver with the given id exists.
 * The returned pointer is owned by the catalog and must not be freed.
 */
Driver *catalog_get_driver(Catalog *catalog, int id);

/**
 * Inserts the top N drivers in the given GPtrArray.
 * Drivers are sorted by their score, date of last ride and id.
 * The drivers are not copied, their pointers are inserted.
 * The GPtrArray should be empty and pre-allocated with N elements.
 * Returns the number of drivers inserted. This number can be less than N if there are less than N drivers.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
int catalog_get_top_drivers_with_best_score(Catalog *catalog, int n, GPtrArray *result);

/**
 * Inserts the top N users in the given GPtrArray.
 * Users are sorted by their total distance, date of last ride and username.
 * The users are not copied, their pointers are inserted.
 * The GPtrArray should be empty and pre-allocated with N elements.
 * Returns the number of users inserted. This number can be less than N if there are less than N users.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
int catalog_get_top_users_with_longest_total_distance(Catalog *catalog, int n, GPtrArray *result);

/**
 * Returns the average price of rides in the given city.
 */
double catalog_get_average_price_in_city(Catalog *p_catalog, char *city);

/**
 * Returns the average price of rides between the given dates.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
double catalog_get_average_price_in_date_range(Catalog *catalog, Date start_date, Date end_date);

void catalog_get_top_n_drivers_in_city(Catalog *catalog,int n, char *city, GPtrArray *result) {

/**
 * Adds all the rides whose passengers gave a tip between start_date and end_date to GPtrArray result.
 * The rides are not copied, their pointers are inserted.
 * The GPtrArray result should be empty.
 * The array is sorted by ride's distance, date and then id.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
void catalog_insert_passengers_that_gave_tip_in_date_range(Catalog *catalog, GPtrArray *result, Date start_date, Date end_date);

#endif //LI3_CATALOG_H
