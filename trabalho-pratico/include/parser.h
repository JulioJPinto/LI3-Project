#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include "user.h"
#include "ride.h"
#include "driver.h"
#include "struct_util.h"
#include <glib.h>

typedef enum file_type{
    USERS, DRIVERS, RIDE
} FileType;

typedef void* (*parse_line_function_pointer)(char* string);

void read_file(const char* file_path, parse_line_function_pointer parse_line_function);

void* parse_driver(char* line);

void* parse_ride(char* line);

void* parse_user(char* line);

#endif //LI3_STRUCT_UTIL_H