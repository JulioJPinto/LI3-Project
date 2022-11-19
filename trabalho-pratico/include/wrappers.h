#pragma once
#ifndef LI3_WRAPPERS_H
#define LI3_WRAPPERS_H

#include "driver.h"
#include "ride.h"
#include "user.h"

/*
 * Wrapper function for the Parse line user function
 */
void *wrapper_voidp_parse_user(char *arg);

/*
 * Wrapper function for the Parse line driver function
 */
void *wrapper_voidp_parse_driver(char *arg);

/*
 * Wrapper function for the Parse line ride function
 */
void *wrapper_voidp_parse_ride(char *arg);

#endif //LI3_WRAPPERS_H