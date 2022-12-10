#include "driver_city_info.h"

#include <glib.h>

struct DriverCityInfo {
    int id;
    char *name;
    int accumulated_score;
    int amount_rides;
};

DriverCityInfo *create_driver_by_city(int id, char *name) {
    DriverCityInfo *driver_by_city = malloc(sizeof(struct DriverCityInfo));
    driver_by_city->id = id;
    driver_by_city->name = g_strdup(name);

    driver_by_city->accumulated_score = 0;
    driver_by_city->amount_rides = 0;
    return driver_by_city;
}

int driver_by_city_get_id(DriverCityInfo *driver) {
    return driver->id;
}

char *driver_by_city_get_name(DriverCityInfo *driver) {
    return driver->name;
}

void driver_by_city_increment_number_of_rides(DriverCityInfo *driver) {
    driver->amount_rides++;
}

void driver_by_city_add_score(DriverCityInfo *driver, int score) {
    driver->accumulated_score += score;
}

double driver_by_city_get_average_score(DriverCityInfo *driver) {
    return (double) driver->accumulated_score / (double) driver->amount_rides;
}
