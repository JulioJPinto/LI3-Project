#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

#include "driver.h"
#include "user.h"
#include "ride.h"

int parse_int(char *string);

double parse_double(char *string);

Date parse_date(char *string);

Gender parse_gender(const char *string);

CarClass parse_car_class(const char *string);

AccountStatus parse_acc_status(const char *string);

PaymentMethod parse_pay_method(const char *string);

User *read_users_file(FILE *stream, char *line_buffer, int line_buffer_size);

Driver *read_drivers_file(FILE *stream, char *line_buffer, int line_buffer_size);

Ride *read_rides_file(FILE *stream, char *line_buffer, int line_buffer_size);

#endif //LI3_PARSER_H
