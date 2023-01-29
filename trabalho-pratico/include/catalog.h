#pragma once
#ifndef LI3_CATALOG_H
#define LI3_CATALOG_H

#include <glib.h>

#include "driver.h"
#include "ride.h"
#include "user.h"
#include "driver_city_info.h"

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
 * Returns the city name associated with the given city id.
 */
char *catalog_get_city_name(Catalog *catalog, int city_id);

/**
 * Returns the city id associated with the given city name.
 */
int catalog_get_city_id(Catalog *catalog, char *city);

/**
 * Returns the city id associated with the given city name.
 * If the city is not registered, it is registered and its id is returned.
 */
int catalog_get_or_register_city_id(Catalog *catalog, char *city);

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
void catalog_force_eager_indexing(Catalog *catalog);

/**
 * Returns the user associated with the given user id.
 */
User *catalog_get_user_by_user_id(Catalog *catalog, int user_id);

/**
 * Returns the user associated with the given username.
 */
User *catalog_get_user_by_username(Catalog *catalog, char* username);

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
 */
int query_2_catalog_get_top_drivers_with_best_score(Catalog *catalog, int n, GPtrArray *result);

/**
 * Inserts the top N users in the given GPtrArray.
 * Users are sorted by their total distance, date of last ride and username.
 * The users are not copied, their pointers are inserted.
 * The GPtrArray should be empty and pre-allocated with N elements.
 * Returns the number of users inserted. This number can be less than N if there are less than N users.
 */
int query_3_catalog_get_top_users_with_longest_total_distance(Catalog *catalog, int n, GPtrArray *result);

/**
 * Returns the average price of rides in the given city.
 */
double query_4_catalog_get_average_price_in_city(Catalog *catalog, int city_id);

/**
 * Returns the average price of rides between the given dates.
 * If there are no rides between the given dates, returns -1.
 */
double query_5_catalog_get_average_price_in_date_range(Catalog *catalog, Date start_date, Date end_date);

/**
 * Returns the average distance of rides in the given city between the given dates.
 * If there are no rides in the city between the given dates, returns -1.
 */
double query_6_catalog_get_average_distance_in_city_by_date(Catalog *catalog, Date start_date, Date end_date, int city_id);

/**
 * Returns the top N drivers in the given city.
 * Drivers are sorted by their average score and id.
 */
int query_7_catalog_get_top_n_drivers_in_city(Catalog *catalog, int n, int city_id, GPtrArray *result);

/**
 * Returns the rides whose user and driver have the same gender and the both account ages are above min_account_age.
 * The rides are sorted by driver's account age, user's account age and then id.
 */
int query_8_catalog_get_rides_with_user_and_driver_with_same_gender_above_acc_age(Catalog *catalog, GPtrArray *result, Gender gender, int min_account_age);

/**
 * Adds all the rides whose passengers gave a tip between start_date and end_date to GPtrArray result.
 * The rides are not copied, their pointers are inserted.
 * The GPtrArray result should be empty.
 * The array is sorted by ride's distance, date and then id.
 */
void query_9_catalog_get_passengers_that_gave_tip_in_date_range(Catalog *catalog, GPtrArray *result, Date start_date, Date end_date);

#endif //LI3_CATALOG_H
