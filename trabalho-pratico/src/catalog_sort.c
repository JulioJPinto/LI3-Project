#include "catalog_sort.h"
#include "catalog.h"

/**
 * Compare driver by activeness.
 */
int compare_driver_by_activeness(Driver *a, Driver *b) {
    return (int) driver_get_account_status(a) - (int) driver_get_account_status(b);
}

/**
 * Compare driver by id.
 */
int compare_driver_by_id(Driver *a, Driver *b) {
    return driver_get_id(a) - driver_get_id(b);
}

/**
 * Compare driver by last ride.
 */
int compare_driver_by_last_ride(Driver *a, Driver *b) {
    return date_compare(driver_get_last_ride_date(a), driver_get_last_ride_date(b));
}

/**
 * Compare driver by average score.
 */
int compare_driver_by_score(Driver *a, Driver *b) {
    double average_score_a = driver_get_average_score(a);
    double average_score_b = driver_get_average_score(b);

    return (average_score_a > average_score_b) - (average_score_a < average_score_b);
}

/**
 * Compare user by total distance.
 */
int compare_user_by_total_distance(User *a, User *b) {
    int total_distance_a = user_get_total_distance(a);
    int total_distance_b = user_get_total_distance(b);

    return total_distance_a - total_distance_b;
}

/**
 * Compare user by last ride.
 */
int compare_user_by_last_ride(User *a, User *b) {
    Date last_ride_date_a = user_get_most_recent_ride(a);
    Date last_ride_date_b = user_get_most_recent_ride(b);

    return date_compare(last_ride_date_a, last_ride_date_b);
}

/**
 * Compare user by username.
 */
int compare_user_by_username(User *a, User *b) {
    char *user_username_a = user_get_username(a);
    char *user_username_b = user_get_username(b);

    int result = strcmp(user_username_a, user_username_b);

    free(user_username_a);
    free(user_username_b);

    return result;
}

/**
 * Compare user by activeness.
 */
int compare_user_by_activeness(User *a, User *b) {
    return (int) user_get_account_status(a) - (int) user_get_account_status(b);
}

int glib_wrapper_compare_drivers_by_score(gconstpointer a, gconstpointer b) {
    Driver *a_driver = *((Driver **) a);
    Driver *b_driver = *((Driver **) b);

    int by_activeness = compare_driver_by_activeness(a_driver, b_driver);
    if (by_activeness != 0) {
        return by_activeness;
    }

    int by_score = compare_driver_by_score(b_driver, a_driver);
    if (by_score != 0) {
        return by_score;
    }

    int by_last_ride = compare_driver_by_last_ride(b_driver, a_driver);
    if (by_last_ride != 0) {
        return by_last_ride;
    }

    return compare_driver_by_id(b_driver, a_driver);
}

int glib_wrapper_compare_users_by_total_distance(gconstpointer a, gconstpointer b) {
    User *a_user = *((User **) a);
    User *b_user = *((User **) b);

    int by_activeness = compare_user_by_activeness(a_user, b_user);
    if (by_activeness != 0) {
        return by_activeness;
    }

    int by_total_distance = compare_user_by_total_distance(b_user, a_user);
    if (by_total_distance != 0) {
        return by_total_distance;
    }

    int by_last_ride = compare_user_by_last_ride(b_user, a_user);
    if (by_last_ride != 0) {
        return by_last_ride;
    }

    return compare_user_by_username(a_user, b_user);
}

int glib_wrapper_compare_rides_by_date(gconstpointer a, gconstpointer b) {
    Ride *a_ride = *(Ride **) a;
    Ride *b_ride = *(Ride **) b;

    return date_compare(ride_get_date(a_ride), ride_get_date(b_ride));
}

int compare_ride_by_distance(int distance_a, int distance_b){
    return distance_a-distance_b;
}

int compare_ride_by_id(int id_a,int id_b){
    return id_a-id_b;
}

int glib_wrapper_compare_rides_by_distance(gconstpointer a, gconstpointer b){
    Ride *a_ride = *(Ride **) a;
    Ride *b_ride = *(Ride **) b;

    int by_distance = compare_ride_by_distance(ride_get_distance(a_ride),ride_get_distance(b_ride));
    if (by_distance != 0){
        return by_distance;
    }
    int by_date= date_compare(ride_get_date(a_ride),ride_get_date(a_ride));
    if (by_date != 0){
        return by_date;
    }
    int by_id= compare_ride_by_id(ride_get_id(a_ride),ride_get_id(b_ride));
        return by_id;
}
