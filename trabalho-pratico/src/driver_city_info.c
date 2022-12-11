#include "driver_city_info.h"

#include <glib.h>

struct DriverCityInfo {
    int id;
    char *name;
    int accumulated_score;
    int amount_rides;
};

DriverCityInfo *create_driver_city_info(int id, char *name) {
    DriverCityInfo *driver_by_city = malloc(sizeof(struct DriverCityInfo));
    driver_by_city->id = id;
    driver_by_city->name = g_strdup(name);

    driver_by_city->accumulated_score = 0;
    driver_by_city->amount_rides = 0;
    return driver_by_city;
}

int driver_city_info_get_id(DriverCityInfo *driver_city_info) {
    return driver_city_info->id;
}

char *driver_city_info_get_name(DriverCityInfo *driver_city_info) {
    return driver_city_info->name;
}

void driver_city_info_register_ride_score(DriverCityInfo *driver_city_info, int score) {
    driver_city_info->accumulated_score += score;
    driver_city_info->amount_rides++;
}

double driver_city_info_get_average_score(DriverCityInfo *driver_city_info) {
    return (double) driver_city_info->accumulated_score / (double) driver_city_info->amount_rides;
}

void free_driver_city_info(DriverCityInfo *driver_city_info) {
    free(driver_city_info->name);
    free(driver_city_info);
}
