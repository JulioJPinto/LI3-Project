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
Ride *create_ride(int id, Date date, int driver_id, char *username, char *city, int distance, int score_user, int score_driver, double tip);

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
 * Returns a copy of the user's username of the ride
 * The caller is responsible for freeing the memory allocated for the username
 */
char *ride_get_user_username(Ride *ride);

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
char *ride_get_city(Ride *ride);

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

#endif //LI3_RIDE_H
