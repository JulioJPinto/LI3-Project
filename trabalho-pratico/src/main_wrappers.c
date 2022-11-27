#include "main_wrappers.h"

#include "catalog.h"

/**
 * Functions to leverage parser calls to catalog and create functions
 */

/**
 * Function to wrap parse_line_user in void pointers
 */
void *wrapper_voidp_parse_user(char *arg, char delimiter) {
    return parse_line_user(arg, delimiter);
}

/**
 * Function to wrap parse_line_driver in void pointers
 */
void *wrapper_voidp_parse_driver(char *arg, char delimiter) {
    return parse_line_driver(arg, delimiter);
}

/**
 * Function to wrap parse_line_ride in void pointers
 */
void *wrapper_voidp_parse_ride(char *arg, char delimiter) {
    return parse_line_ride(arg, delimiter);
}

/**
 * Function to wrap register_user in void pointers
 */
void wrapper_voidp_register_user(void *catalog, void *user) {
    register_user((Catalog *) catalog, (User *) user);
}

/**
 * Function to wrap register_driver in void pointers
 */
void wrapper_voidp_register_driver(void *catalog, void *driver) {
    register_driver((Catalog *) catalog, (Driver *) driver);
}

/**
 * Function to wrap register_ride in void pointers
 */
void wrapper_voidp_register_ride(void *catalog, void *ride) {
    register_ride((Catalog *) catalog, (Ride *) ride);
}
