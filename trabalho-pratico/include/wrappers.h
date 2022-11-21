#pragma once
#ifndef LI3_WRAPPERS_H
#define LI3_WRAPPERS_H

void *wrapper_voidp_parse_user(char *arg, char* delimiter);

void *wrapper_voidp_parse_driver(char *arg, char* delimiter);

void *wrapper_voidp_parse_ride(char *arg, char* delimiter);

void wrapper_voidp_register_user(void *catalog, void *user);

void wrapper_voidp_register_driver(void *catalog, void *driver);

void wrapper_voidp_register_ride(void *catalog, void *ride);

#endif //LI3_WRAPPERS_H
