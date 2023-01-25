#pragma once
#ifndef LI3_DRIVER_CITY_INFO_H
#define LI3_DRIVER_CITY_INFO_H

typedef struct DriverCityInfo DriverCityInfo;

DriverCityInfo *create_driver_city_info(int id);

int driver_city_info_get_id(DriverCityInfo *driver_city_info);

void driver_city_info_register_ride_score(DriverCityInfo *driver_city_info, int score);

double driver_city_info_get_average_score(DriverCityInfo *driver_city_info);

void free_driver_city_info_voidp(void *driver_city_info);

/**
 * Function that compares DriverCityInfos by average score and id.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the DriverCityInfos array for fast resolution of the query 7.
 */
int compare_driver_city_infos_by_average_score(const void *a_driver_city_info, const void *b_driver_city_info);

#endif //LI3_DRIVER_CITY_INFO_H
