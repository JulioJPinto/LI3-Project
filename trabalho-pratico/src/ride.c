#include "ride.h"

#include <glib.h>
#include "struct_util.h"
#include "string_util.h"

/**
 * Struct that represents a ride.
 */
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
    Date user_account_creation_date;
    Date driver_account_creation_date;
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

Ride *parse_line_ride(char *line, char delim) {
    return parse_line_ride_detailed(line, delim, NULL, NULL);
}

Ride *parse_line_ride_detailed(char *line, char delim, char **parsed_city, char **parsed_user_username) {
    char *id_string = next_token(&line, delim);
    if (IS_EMPTY(id_string)) return NULL;
    int id = parse_int_unsafe(id_string);

    char *date_string = next_token(&line, delim);
    Date date = parse_date(date_string);
    if (!is_date_valid(date)) return NULL;

    char *driver_string = next_token(&line, delim);
    if (IS_EMPTY(driver_string)) return NULL;
    int driver_id = parse_int_unsafe(driver_string);

    char *user = next_token(&line, delim);
    if (IS_EMPTY(user)) return NULL;

    char *city = next_token(&line, delim);
    if (IS_EMPTY(city)) return NULL;

    int error = 0;

    char *distance_string = next_token(&line, delim);
    if (IS_EMPTY(distance_string)) return NULL;
    int distance = parse_int_safe(distance_string, &error);

    char *user_score_string = next_token(&line, delim);
    if (IS_EMPTY(user_score_string)) return NULL;
    int user_score = parse_int_safe(user_score_string, &error);

    char *driver_score_string = next_token(&line, delim);
    if (IS_EMPTY(driver_score_string)) return NULL;
    int driver_score = parse_int_safe(driver_score_string, &error);

    char *tip_string = next_token(&line, delim);
    if (IS_EMPTY(tip_string)) return NULL;
    double tip = parse_double_safe(tip_string, &error);

    if (error || distance < 1 || user_score < 1 || driver_score < 1 || tip < 0 || tip != tip) return NULL;

    if (parsed_city) *parsed_city = city;
    if (parsed_user_username) *parsed_user_username = user;

    return create_ride(id, date, driver_id, user, city, distance, user_score, driver_score, tip);
}

void free_ride(Ride *ride) {
    free(ride->user_username);
    free(ride->city);
    free(ride);
}

int ride_get_driver_id(Ride *ride) {
    return ride->driver_id;
}

int ride_get_id(Ride *ride) {
    return ride->id;
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

void ride_set_user_account_creation_date(Ride *ride, Date date) {
    ride->user_account_creation_date = date;
}

Date ride_get_user_account_creation_date(Ride *ride) {
    return ride->user_account_creation_date;
}

void ride_set_driver_account_creation_date(Ride *ride, Date date) {
    ride->driver_account_creation_date = date;
}

Date ride_get_driver_account_creation_date(Ride *ride) {
    return ride->driver_account_creation_date;
}
