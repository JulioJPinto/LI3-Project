#pragma once
#ifndef LI3_DRIVER_CITY_INFO_H
#define LI3_DRIVER_CITY_INFO_H

typedef struct DriverCityInfo DriverCityInfo;

DriverCityInfo *create_driver_city_info(int id, char *name);

int driver_city_info_get_id(DriverCityInfo *driver_city_info);

char *driver_city_info_get_name(DriverCityInfo *driver_city_info);

void driver_city_info_register_ride_score(DriverCityInfo *driver_city_info, int score);

int driver_city_info_get_accumulated_score(DriverCityInfo *driver);

double driver_city_info_get_average_score(DriverCityInfo *driver);

void free_driver_city_info(DriverCityInfo *driver_city_info);

#endif //LI3_DRIVER_CITY_INFO_H
