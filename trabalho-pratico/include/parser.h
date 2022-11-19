#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

#include "struct_util.h"

typedef void *(ParserLineFunction) (char *);

typedef void(ApplyFunction)(void *, void *);

int parse_int(char *string);

double parse_double(char *string);

Date parse_date(char *string);

Gender parse_gender(const char *string);

CarClass parse_car_class(const char *string);

AccountStatus parse_acc_status(const char *string);

PaymentMethod parse_pay_method(const char *string);

void read_file(FILE *stream, ParserLineFunction *parse_line_function, ApplyFunction *apply_function, void *apply_function_first_arg);

#endif //LI3_PARSER_H
