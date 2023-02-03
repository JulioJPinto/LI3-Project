#pragma once
#ifndef LI3_DRIVER_H
#define LI3_DRIVER_H

#include "struct_util.h"
#include "token_iterator.h"

/**
 * Struct that represents a driver.
 */
typedef struct Driver Driver;

/**
 * Creates a new Driver.
 */
Driver *create_driver(int id, char *name, Date birth_date, Gender gender, CarClass car_class, const char *license_plate,
                      Date account_creation_date, AccountStatus account_status);

/**
 * Parses a line of the CSV to a driver
 * parsed_city is used to return the city name of the ride because
 * the driver saves a city id and not a city name
 */
Driver *parse_line_driver_detailed(TokenIterator *line_iterator, char **parsed_city);

/**
 + Parses a line of the CSV to a driver   
 */
Driver *parse_line_driver(TokenIterator *line_iterator);

/**
 * Frees the memory allocated for the Driver.
 */
void free_driver(void *driver);

/**
 * Returns the id of the Driver
 */
int driver_get_id(Driver *driver);

/**
 * Returns a copy of the name of the Driver
 * The caller is responsible for freeing the memory allocated for the name
 */
char *driver_get_name(Driver *driver);

/**
 * Sets the city id of the Driver
 * The city id is set when the driver is registered in the catalog
 */
void driver_set_city_id(Driver *driver, int city_id);

/**
 * Returns the city id of the Driver
 */
int driver_get_city_id(Driver *driver);

/**
 * Returns the birthdate of the Driver
 */
Date driver_get_birthdate(Driver *driver);

/**
 * Returns the gender of the Driver
 */
Gender driver_get_gender(Driver *driver);

/**
 * Returns the car class of the Driver
 */
CarClass driver_get_car_class(Driver *driver);

/**
 * Returns the account creation date of the Driver
 */
Date driver_get_account_creation_date(Driver *driver);

/**
 * Returns the account status of the Driver
 */
AccountStatus driver_get_account_status(Driver *driver);

/**
 * Returns the date of the last ride of the Driver
 */
Date driver_get_last_ride_date(Driver *driver);

/**
 * Increments the rides amount of the Driver
 */
void driver_increment_number_of_rides(Driver *driver);

/**
 * Increments the accumulated score of the Driver
 */
void driver_add_score(Driver *driver, int score);

/**
 * Increments the total money earned by the Driver
 */
void driver_add_earned(Driver *driver, double earned);

/**
 * Returns the average score of the Driver
 */
double driver_get_average_score(Driver *driver);

/**
 * Returns the number of rides of the Driver
 */
int driver_get_number_of_rides(Driver *driver);

/**
 * Returns the total money earned by the Driver
 */
double driver_get_total_earned(Driver *driver);

/**
 * Sets a new last ride if the new ride is more recent than the current saved one
 */
void driver_register_ride_date(Driver *driver, Date date);

/**
 * Function that compares drivers by activeness, score, last ride and id.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the drivers array for fast resolution of the query 2.
 */
int compare_drivers_by_score(const void *a_driver, const void *b_driver);

#endif //LI3_DRIVER_H
