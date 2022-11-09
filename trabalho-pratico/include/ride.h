#pragma once
#ifndef LI3_RIDE_H
#define LI3_RIDE_H

typedef struct ride Ride;

Ride *create_ride();

GString *ride_get_city(Ride *ride);

#endif //LI3_RIDE_H
