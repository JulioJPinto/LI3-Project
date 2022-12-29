#pragma once
#ifndef LI3_CATALOG_H
#define LI3_CATALOG_H

#include <glib.h>

#include "driver.h"
#include "ride.h"
#include "user.h"
#include "driver_city_info.h"
#include "ride_driver_and_user_info.h"

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
 * Receives a catalog as void pointer to be used as a generic function.
 */
void parse_and_register_user(void *catalog, char *line, char separator);

/**
 * Registers a driver in the catalog.
 * Receives a catalog as void pointer to be used as a generic function.
 */
void parse_and_register_driver(void *catalog, char *line, char separator);

/**
 * Registers a ride in the catalog.
 * The user and driver linked to the ride must be already registered in the catalog
 * as it updates the user and driver information related to this ride.
 * Receives a catalog as void pointer to be used as a generic function.
 */
void parse_and_register_ride(void *catalog, char *line, char separator);

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
int query_2_catalog_get_top_drivers_with_best_score(Catalog *catalog, int n, GPtrArray *result);

/**
 * Inserts the top N users in the given GPtrArray.
 * Users are sorted by their total distance, date of last ride and username.
 * The users are not copied, their pointers are inserted.
 * The GPtrArray should be empty and pre-allocated with N elements.
 * Returns the number of users inserted. This number can be less than N if there are less than N users.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
int query_3_catalog_get_top_users_with_longest_total_distance(Catalog *catalog, int n, GPtrArray *result);

/**
 * Returns true if the city is registered in the catalog.
 * False otherwise.
 */
gboolean catalog_city_exists(Catalog *catalog, char *city);

/**
 * Returns the average price of rides in the given city.
 */
double query_4_catalog_get_average_price_in_city(Catalog *catalog, char *city);

/**
 * Returns the average price of rides between the given dates.
 * If there are no rides between the given dates, returns -1.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
double query_5_catalog_get_average_price_in_date_range(Catalog *catalog, Date start_date, Date end_date);

/**
 * Returns the average distance of rides in the given city between the given dates.
 * If there are no rides in the city between the given dates, returns -1.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
double query_6_catalog_get_average_distance_in_city_by_date(Catalog *catalog, Date start_date, Date end_date, char *city);

int query_7_catalog_get_top_n_drivers_in_city(Catalog *catalog, int n, char *city, GPtrArray *result);

int query_8_catalog_get_rides_with_user_and_driver_with_same_gender_above_acc_age(Catalog *catalog, GPtrArray *result, Gender gender, int min_account_age);

/**
 * Adds all the rides whose passengers gave a tip between start_date and end_date to GPtrArray result.
 * The rides are not copied, their pointers are inserted.
 * The GPtrArray result should be empty.
 * The array is sorted by ride's distance, date and then id.
 * `notify_stop_registering(Catalog*)` should be called before calling this function.
 */
void query_9_catalog_get_passengers_that_gave_tip_in_date_range(Catalog *catalog, GPtrArray *result, Date start_date, Date end_date);

#endif //LI3_CATALOG_H
