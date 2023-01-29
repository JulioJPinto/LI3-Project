#pragma once
#ifndef LI3_DRIVER_CITY_INFO_H
#define LI3_DRIVER_CITY_INFO_H

/**
 * Struct that represents a driver city info. A driver city info contains information about a driver in a particular city.
 */
typedef struct DriverCityInfo DriverCityInfo;

/**
 * Creates a new DriverCityInfo with the given id.
 */
DriverCityInfo *create_driver_city_info(int id);

/**
 * Returns the id of the driver associated with the DriverCityInfo.
 */
int driver_city_info_get_id(DriverCityInfo *driver_city_info);

/**
 * Registers a ride score in the DriverCityInfo.
 * This function updates the total accumulated score of the DriverCityInfo.
 */
void driver_city_info_register_ride_score(DriverCityInfo *driver_city_info, int score);

/**
 * Returns the average score of the DriverCityInfo.
 */
double driver_city_info_get_average_score(DriverCityInfo *driver_city_info);

/**
 * Frees the memory allocated for the DriverCityInfo.
 */
void free_driver_city_info_voidp(void *driver_city_info);

/**
 * Function that compares DriverCityInfos by average score and id.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the DriverCityInfos array for fast resolution of the query 7.
 */
int compare_driver_city_infos_by_average_score(const void *a_driver_city_info, const void *b_driver_city_info);

#endif //LI3_DRIVER_CITY_INFO_H
