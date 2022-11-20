#include <glib.h>

#include "ride.h"
#include "struct_util.h"
#include "parser.h"

struct Ride {
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

    // Query specific
    double price;
};

Ride *create_ride(int id, Date date, int driver_id, char *username, char *city, int distance, int score_user, int score_driver, double tip) {
    Ride *ride = malloc(sizeof(Ride));

    ride->id = id;
    ride->date = date;
    ride->driver_id = driver_id;
    ride->user_username = g_strdup(username);
    ride->city = g_strdup(city);
    ride->distance = distance;
    ride->score_user = score_user;
    ride->score_driver = score_driver;
    ride->tip = tip;

    return ride;
}

Ride *parse_line_ride(char *line, char* delim) {
    char *id_string = strtok(line, delim);
    int id = parse_int(id_string);

    char *date_string = strtok(NULL, delim);
    Date date = parse_date(date_string);

    char *driver_string = strtok(NULL, delim);
    int driver_id = parse_int(driver_string);

    char *user = strtok(NULL, delim);

    char *city = strtok(NULL, delim);

    char *distance_string = strtok(NULL, delim);
    int distance = parse_int(distance_string);

    char *user_score_string = strtok(NULL, delim);
    int user_score = parse_int(user_score_string);

    char *driver_score_string = strtok(NULL, delim);
    int driver_score = parse_int(driver_score_string);

    char *tip_string = strtok(NULL, delim);
    double tip = parse_double(tip_string);

    //    char *comment = strtok(NULL, delim);;

    return create_ride(id, date, driver_id, user, city, distance, user_score, driver_score, tip);
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
    return g_strdup(ride->user_username);
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
    return g_strdup(ride->city);
}

void ride_set_price(Ride *ride, double price) {
    ride->price = price;
}

double ride_get_price(Ride *ride) {
    return ride->price;
}
