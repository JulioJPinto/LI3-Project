#pragma once
#ifndef LI3_STRUCT_UTIL_H
#define LI3_STRUCT_UTIL_H

#include <stdint.h>

/**
 * Struct that represents a gender (Female and Male)
 */
typedef enum gender {
    M,
    F
} Gender;

/**
 * Struct that represents a date (day, month and year)
 */
typedef struct date {
    uint32_t encoded_date;
} Date;

/**
 * Struct that represents a payment method (Cash, Debit and Credit)
 */
typedef enum payment_method {
    CASH,
    DEBIT,
    CREDIT
} PaymentMethod;

/**
 * Struct that represents an account status (Active and Inactive)
 */
typedef enum account_status {
    INVALID_ACCOUNT_STATUS = -1, // only used for error handling in parsing
    ACTIVE,
    INACTIVE,
} AccountStatus;


/**
 * Struct that represents a car class (Basic, Green and Premium)
 */
typedef enum car_class {
    INVALID_CAR_CLASS = -1, // only used for error handling in parsing
    BASIC,
    GREEN,
    PREMIUM,
} CarClass;

/**
 * Calculates the age of a person given their date of birth
 * The age is calculated based on the reference date (9/10/2022)
 */
int get_age(Date date_of_birth);

/**
 * Compares two dates
 * Returns a negative number if date_1 is before date_2
 * Returns a positive number if date_1 is after date_2
 * Returns 0 if date_1 is equal to date_2
 */
int date_compare(Date date_1, Date date_2);

/**
 * Parses a string into an int
 * Returns the int value of the string
 * Sets error to 1 if the string is not a valid int
 */
int parse_int_safe(char *string, int *error);

/**
 * Parses a string into an int assuming it is a valid int
 */
int parse_int_unsafe(char *string);

/**
 * Parses a string into an int
 * Returns the int value of the string
 * Sets error to 1 if the string is not a valid int
 */
double parse_double_safe(char *string, int *error);

/**
 * Returns 1 if the date is valid, 0 otherwise
 */
int is_date_valid(Date date);

/**
 * Parses a string into a date struct
 * Returns an invalid date if the string is not a valid date
 * The date is considered valid if it is in the format dd/mm/yyyy (1<=dd<=12 && 1<=mm<=31)
 */
Date parse_date(char *string);

/**
 * Creates a date struct from the given day, month and year
 */
Date create_date(int day, int month, int year);

int date_get_day(Date date);

int date_get_month(Date date);

int date_get_year(Date date);

/**
 * Parses a string into a gender struct 
 */
Gender parse_gender(const char *string);

/**
 * Parses a string into a car class struct
 * Returns INVALID_CAR_CLASS if the string is not a valid car class
 * This will modify the string to be all uppercase
 */
CarClass parse_car_class(char *string);

/**
 * Parses a string into an account status struct
 * Returns INVALID_ACCOUNT_STATUS if the string is not a valid account status
 * This will modify the string to be all uppercase
 */
AccountStatus parse_acc_status(char *string);

/**
 * Parses a string into a payment method struct
 */
PaymentMethod parse_pay_method(const char *string);

#endif //LI3_STRUCT_UTIL_H
