#pragma once
#ifndef LI3_RIDE_H
#define LI3_RIDE_H

#include "struct_util.h"

/**
 * Struct that represents a user.
 */
typedef struct Ride Ride;

/**
 * Creates a new Ride.
 */
Ride *create_ride(int id, Date date, int driver_id, int city_id, int distance, int score_user, int score_driver, double tip);

/**
 * Parses a line of the CSV to a ride
 */
Ride *parse_line_ride(char *line, char delim);

/**
 * Parses a line of the CSV to a ride
 * parsed_city and parsed_user_username are used to return the strings city and user_username of the ride
 * to avoid having to strdup them again when registering the ride in the catalog
 */
Ride *parse_line_ride_detailed(char *line, char delim, char **parsed_city, char **parsed_user_username);

/**
 * Sets the city id of the ride
 * The city id is set when the ride is registered in the catalog
 */
void ride_set_city_id(Ride *ride, int city_id);

/**
 * Returns the driver id of the ride
 */
int ride_get_driver_id(Ride *ride);

/**
 * Returns the id of the ride
 */
int ride_get_id(Ride *ride);

/**
 * Returns the date of the ride
 */
Date ride_get_date(Ride *ride);

/**
 * Returns the user id of the ride
 * The user id is set when the ride is registered in the catalog
 */
int ride_get_user_id(Ride *ride);

/**
 * Sets the user id of the ride
 * The user id is set when the ride is registered in the catalog
 */
void ride_set_user_id(Ride *ride, int user_id);

/**
 * Returns the distance of the ride
 */
int ride_get_distance(Ride *ride);

/**
 * Returns the score given by the driver to the user
 */
int ride_get_score_user(Ride *ride);

/**
 * Returns the score given by the user to the driver
 */
int ride_get_score_driver(Ride *ride);

/**
 * Returns the tip given by the user to the driver
 */
double ride_get_tip(Ride *ride);

/**
 * Frees the memory allocated for the Ride.
 */
void free_ride(Ride *ride);

/**
 * Returns the city of the Ride.
 */
int ride_get_city_id(Ride *ride);

/**
 * Sets the price of the ride
 * The price is calculated by the function `compute_price` when the ride is registered in the catalog
 */
void ride_set_price(Ride *ride, double price);

/**
 * Returns the price of the ride
 * The price is calculated and set when the ride is registered in the catalog
 */
double ride_get_price(Ride *ride);

/**
 * Sets the user account creation date of the ride
 * The date is set when the ride is registered in the catalog
 */
void ride_set_user_account_creation_date(Ride *ride, Date date);

/**
 * Returns the user account creation date of the ride
 */
Date ride_get_user_account_creation_date(Ride *ride);

/**
 * Sets the driver account creation date of the ride
 * The date is set when the ride is registered in the catalog
 */
void ride_set_driver_account_creation_date(Ride *ride, Date date);

/**
 * Returns the driver account creation date of the ride
 */
Date ride_get_driver_account_creation_date(Ride *ride);

/**
 * Function that compares rides by date.
 * This function receives const pointers to be used as comparison functions.
 * Used to sort the rides array for fast resolution of the query 5 and 6.
 */
int compare_rides_by_date(const void *a_ride, const void *b_ride);

/**
 * Function that compares RideDriverAndUserInfos by driver account creation date, user account creation date and then ride id.
 * This function receives const pointers to be used as comparison functions.
 * Used to sort the rides array for fast resolution of the query 8.
 */
int compare_ride_by_driver_and_user_account_creation_date(const void *a_ride, const void *b_ride);

/**
 * Function that compares rides by total distance, date, and then id.
 * This function receives const pointers to be used as comparison functions.
 * Used to sort the rides array for fast resolution of the query 9.
 */
int compare_rides_by_distance(const void *a_ride, const void *b_ride);

#endif //LI3_RIDE_H
