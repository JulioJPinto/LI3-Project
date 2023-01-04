#include "ride_driver_and_user_info.h"

#include <glib.h>
#include <stdlib.h>

struct RideDriverAndUserInfo {
    int ride_id;
    Date user_account_creation_date;
    Date driver_account_creation_date;
};

RideDriverAndUserInfo *create_rduinfo(int ride_id,
                                      Date user_account_creation_date,
                                      Date driver_account_creation_date) {
    RideDriverAndUserInfo *ride_driver_and_user_info = malloc(sizeof(RideDriverAndUserInfo));
    ride_driver_and_user_info->ride_id = ride_id;
    ride_driver_and_user_info->user_account_creation_date = user_account_creation_date;
    ride_driver_and_user_info->driver_account_creation_date = driver_account_creation_date;
    return ride_driver_and_user_info;
}

void free_rduinfo_internal(RideDriverAndUserInfo *ride_driver_and_user_info) {
    free(ride_driver_and_user_info->user_username);
    free(ride_driver_and_user_info);
}

void free_rduinfo(void *ride_driver_and_user_info) {
    free_rduinfo_internal(ride_driver_and_user_info);
}

int rduinfo_get_ride_id(RideDriverAndUserInfo *ride_driver_and_user_info) {
    return ride_driver_and_user_info->ride_id;
}

Date rduinfo_get_user_account_creation_date(RideDriverAndUserInfo *ride_driver_and_user_info) {
    return ride_driver_and_user_info->user_account_creation_date;
}

Date rduinfo_get_driver_account_creation_date(RideDriverAndUserInfo *ride_driver_and_user_info) {
    return ride_driver_and_user_info->driver_account_creation_date;
}
