#include <glib.h>

#include "ride.h"
#include "struct_util.h"

typedef struct ride {
    int id;
    Date date;
    int driver_id;
    char* user_username;
    char* city;
    int distance;
    int score_user;
    int score_driver;
    float tip;
    // GString comment;
} Ride;

Ride* create_ride(int id, Date date, int driver_id, char* username, char* city, int distance, int score_user, int score_driver, float tip){
    Ride *ride = malloc(sizeof(Ride));

    ride->id = id;
    ride->date = date;
    ride->driver_id = driver_id;
    ride->user_username = username;
    ride->city = city;
    ride->distance = distance;
    ride->score_user = score_user;
    ride->score_driver = score_driver;
    ride->tip = tip;

    return ride;
}

void free_ride(Ride *ride){
    free(ride);
}

char* *ride_get_city(Ride *ride) {
    return *(ride->city);
}
