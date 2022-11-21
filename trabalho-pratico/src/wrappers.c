#include "wrappers.h"

#include "catalog.h"

void *wrapper_voidp_parse_user(char *arg, char* delimiter) {
    return parse_line_user(arg, delimiter);
}

void *wrapper_voidp_parse_driver(char *arg, char* delimiter) {
    return parse_line_driver(arg, delimiter);
}

void *wrapper_voidp_parse_ride(char *arg, char* delimiter) {
    return parse_line_ride(arg, delimiter);
}

void wrapper_voidp_register_user(void *catalog, void *user) {
    register_user((Catalog *) catalog, (User *) user);
}

void wrapper_voidp_register_driver(void *catalog, void *driver) {
    register_driver((Catalog *) catalog, (Driver *) driver);
}

void wrapper_voidp_register_ride(void *catalog, void *ride) {
    register_ride((Catalog *) catalog, (Ride *) ride);
}
