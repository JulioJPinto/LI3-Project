#pragma once
#ifndef LI3_DRIVER_H
#define LI3_DRIVER_H

#include "struct_util.h"
#include <glib.h>

typedef struct Driver Driver;

/*
 * Creates a new Driver.
 */
Driver *create_driver(int id, char *name, Date birth_date, Gender gender, CarClass car_class, char *license_plate,
                      char *city, Date account_creation_date, AccountStatus account_status);

/*
 * Frees the memory allocated for the Driver.
 */
void free_driver(Driver *driver);

/*
 * Returns the id of the Driver
 */
int driver_get_id(Driver *driver);

/*
 * Returns the name of the Driver
 */
char *driver_get_name(Driver *driver);

/*
 * Returns the birthdate of the Driver
 */
Date driver_get_birthdate(Driver *driver);

/*
 * Returns the gender of the Driver
 */
Gender driver_get_gender(Driver *driver);

/*
 * Returns the car class of the Driver
 */
CarClass driver_get_car_class(Driver *driver);

/*
 * Returns the account status of the Driver
 */
AccountStatus driver_get_account_status(Driver *driver);

/*
 * Returns the date of the last ride of the Driver
 */
Date driver_get_last_ride_date(Driver *driver);

/*
 * Increments the rides amount of the Driver
 */
void driver_increment_number_of_rides(Driver *driver);

/*
 * Increments the accumulated score of the Driver
 */
void driver_add_score(Driver *driver, int score);

/*
 * Increments the total money earned by the Driver
 */
void driver_add_earned(Driver *driver, double earned);

/*
 * Returns the average score of the Driver
 */
double driver_get_average_score(Driver *driver);

/*
 * Returns the number of rides of the Driver
 */
int driver_get_number_of_rides(Driver *driver);

/*
 * Returns the total money earned by the Driver
 */
double driver_get_total_earned(Driver *driver);

/*
 * Sets a new last ride if the new ride is more recent than the current saved one
 */
void driver_register_last_ride_date(Driver *driver, Date date);

void* wrapper_voidp_parse_driver(char **arg, void* driver);

#endif //LI3_DRIVER_H
