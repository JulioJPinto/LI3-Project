#pragma once
#ifndef LI3_RIDE_DRIVER_AND_USER_INFO_H
#define LI3_RIDE_DRIVER_AND_USER_INFO_H

#include "struct_util.h"

// Query 8 data structures
// Optimized for cpu time instead of memory

typedef struct RideDriverAndUserInfo RideDriverAndUserInfo;

RideDriverAndUserInfo *create_rduinfo(int ride_id,
                                      Date user_account_creation_date,
                                      Date driver_account_creation_date);

int rduinfo_get_ride_id(RideDriverAndUserInfo *ride_driver_and_user_info);

Date rduinfo_get_user_account_creation_date(RideDriverAndUserInfo *ride_driver_and_user_info);

Date rduinfo_get_driver_account_creation_date(RideDriverAndUserInfo *ride_driver_and_user_info);

void free_rduinfo(void *ride_driver_and_user_info);

#endif //LI3_RIDE_DRIVER_AND_USER_INFO_H
