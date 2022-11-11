#pragma once
#ifndef LI3_RIDE_H
#define LI3_RIDE_H

#include "struct_util.h"
#include <glib.h>

typedef struct ride Ride;

Ride* create_ride(int id, Date date, int driver_id, char* username, char* city, int distance, int score_user, int score_driver, float tip);

void free_ride(Ride *ride);

GString *ride_get_city(Ride *ride);

#endif //LI3_RIDE_H
