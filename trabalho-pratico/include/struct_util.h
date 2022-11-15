#pragma once
#ifndef LI3_STRUCT_UTIL_H
#define LI3_STRUCT_UTIL_H

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

#endif //LI3_STRUCT_UTIL_H
