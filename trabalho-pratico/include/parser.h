#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include "user.h"
#include "ride.h"
#include "driver.h"
#include "struct_util.h"
#include <glib.h>
#include <stdio.h>

typedef enum file_type {
    USERS,
    DRIVERS,
    RIDE
} FileType;

typedef void *(*parse_line_function_pointer)(char *string);

void parse_file(FILE *file_path, parse_line_function_pointer parse_line_function);

void *parse_driver_line(char *line);

void *parse_ride_line(char *line);

void *parse_user_line(char *line);

#endif //LI3_STRUCT_UTIL_H