#pragma once
#ifndef LI3_STRUCT_UTIL_H
#define LI3_STRUCT_UTIL_H

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
    int day, month, year;
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
    ACTIVE,
    INACTIVE
} AccountStatus;


/**
 * Struct that represents a car class (Basic, Green and Premium)
 */
typedef enum car_class {
    BASIC,
    GREEN,
    PREMIUM
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
 */
int parse_int(char *string);

/**
 * Parses a string into a double 
 */
double parse_double(char *string);

/**
 * Parses a string into a date struct 
 */
Date parse_date(char *string);

/**
 * Parses a string into a gender struct 
 */
Gender parse_gender(const char *string);

/**
 * Parses a string into a car class struct 
 */
CarClass parse_car_class(const char *string);

/**
 * Parses a string into an account status struct 
 */
AccountStatus parse_acc_status(const char *string);

/**
 * Parses a string into a payment method struct 
 */
PaymentMethod parse_pay_method(const char *string);

#endif //LI3_STRUCT_UTIL_H
