#pragma once
#ifndef LI3_STRUCT_UTIL_H
#define LI3_STRUCT_UTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum gender {
    M,
    F
} Gender;

typedef struct date {
    int day, month, year;
} Date;

int get_age(Date date_of_birth);

int date_compare(Date date_1, Date date_2);

typedef enum payment_method {
    CASH,
    DEBIT,
    CREDIT
} PaymentMethod;

typedef enum account_status {
    ACTIVE,
    INACTIVE
} AccountStatus;

typedef enum car_class {
    BASIC,
    GREEN,
    PREMIUM
} CarClass;

/*
 + Parses a string into an int
 */
int parse_int(char *string);

/*
 + Parses a string into a double 
 */
double parse_double(char *string);

/*
 + Parses a string into a date struct 
 */
Date parse_date(char *string);

/*
 + Parses a string into a gender struct 
 */
Gender parse_gender(const char *string);

/*
 + Parses a string into a car class struct 
 */
CarClass parse_car_class(const char *string);

/*
 + Parses a string into an account status struct 
 */
AccountStatus parse_acc_status(const char *string);

/*
 + Parses a string into a payment method struct 
 */
PaymentMethod parse_pay_method(const char *string);

#endif //LI3_STRUCT_UTIL_H
