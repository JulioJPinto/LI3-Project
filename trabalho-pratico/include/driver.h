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
 * Returns the car class of the Driver
 */
CarClass driver_get_car_class(Driver *driver);

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

#endif //LI3_DRIVER_H
