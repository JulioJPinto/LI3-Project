#pragma once
#ifndef LI3_RIDE_H
#define LI3_RIDE_H

#include "struct_util.h"
#include <glib.h>

typedef struct Ride Ride;

/*
 * Creates a new Ride.
 */
Ride *create_ride(int id, Date date, int driver_id, char *username, char *city, int distance, int score_user, int score_driver, double tip);

/*
 * Returns the driver id of the ride
 */
int ride_get_driver_id(Ride *ride);

/*
 * Returns the date of the ride
 */
Date ride_get_date(Ride *ride);

/*
 * Returns a copy of the user's username of the ride
 * The caller is responsible for freeing the memory allocated for the username
 */
char *ride_get_user_username(Ride *ride);

/*
 * Returns the distance of the ride
 */
int ride_get_distance(Ride *ride);

/*
 * Returns the score given by the driver to the user
 */
int ride_get_score_user(Ride *ride);

/*
 * Returns the score given by the user to the driver
 */
int ride_get_score_driver(Ride *ride);

/*
 * Returns the tip given by the user to the driver
 */
double ride_get_tip(Ride *ride);

/*
 * Frees the memory allocated for the Ride.
 */
void free_ride(Ride *ride);

char *ride_get_city(Ride *ride);

#endif //LI3_RIDE_H
