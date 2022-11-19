#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

#include "driver.h"
#include "user.h"
#include "ride.h"

typedef void* (parser_line_function)(char**, void*);

typedef void *(register_function)(void* arg0, void* arg1);

int parse_int(char *string);

double parse_double(char *string);

Date parse_date(char *string);

Gender parse_gender(const char *string);

CarClass parse_car_class(const char *string);

AccountStatus parse_acc_status(const char *string);

PaymentMethod parse_pay_method(const char *string);

void read_file(FILE *stream, parser_line_function *parse_line_function, void* arg0, register_function* register_function);

#endif //LI3_PARSER_H
