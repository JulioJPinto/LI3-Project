#pragma once
#ifndef LI3_DRIVER_CITY_INFO_H
#define LI3_DRIVER_CITY_INFO_H

typedef struct DriverCityInfo DriverCityInfo;

int driver_by_city_get_id(DriverCityInfo *driver);

char *driver_by_city_get_name(DriverCityInfo *driver);

void driver_by_city_increment_number_of_rides(DriverCityInfo *driver);

void driver_by_city_add_score(DriverCityInfo *driver, int score);

double driver_by_city_get_average_score(DriverCityInfo *driver);

#endif //LI3_DRIVER_CITY_INFO_H
