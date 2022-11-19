#include <glib.h>

#include "wrappers.h"

void *wrapper_voidp_parse_user(char *arg) {
    return parse_line_user(arg);
}

void *wrapper_voidp_parse_driver(char *arg) {
    return parse_line_driver(arg);
}

void *wrapper_voidp_parse_ride(char *arg) {
    return parse_line_ride(arg);
}