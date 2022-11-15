#include <glib.h>

#include "ride.h"
#include "struct_util.h"

typedef struct Ride {
    int id;
    Date date;
    int driver_id;
    char *user_username;
    char *city;
    int distance;
    int score_user;
    int score_driver;
    double tip;
    // GString comment;
} Ride;

Ride *create_ride(int id, Date date, int driver_id, char *username, char *city, int distance, int score_user, int score_driver, double tip) {
    Ride *ride = malloc(sizeof(Ride));

    ride->id = id;
    ride->date = date;
    ride->driver_id = driver_id;
    ride->user_username = g_strdup(username);
    ride->city = city;
    ride->distance = distance;
    ride->score_user = score_user;
    ride->score_driver = score_driver;
    ride->tip = tip;

    return ride;
}

void free_ride(Ride *ride) {
    free(ride->user_username);
    free(ride);
}

int ride_get_driver_id(Ride *ride) {
    return ride->driver_id;
}

Date ride_get_date(Ride *ride) {
    return ride->date;
}

char *ride_get_user_username(Ride *ride) {
    return ride->user_username;
}

int ride_get_distance(Ride *ride) {
    return ride->distance;
}

int ride_get_score_user(Ride *ride) {
    return ride->score_user;
}

int ride_get_score_driver(Ride *ride) {
    return ride->score_driver;
}

double ride_get_tip(Ride *ride) {
    return ride->tip;
}

char *ride_get_city(Ride *ride) {
    return ride->city;
}
