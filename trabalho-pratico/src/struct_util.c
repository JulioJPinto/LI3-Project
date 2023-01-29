#include "struct_util.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <glib.h>

#include "string_util.h"

const Date invalid_date = {.encoded_date = 0};
static const int reference_date_day = 9;
static const int reference_date_month = 10;
static const int reference_date_year = 2022;

/**
 * Returns 1 if the char is a digit, 0 otherwise
 */
static int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int parse_int_safe(char *string, int *error) {
    int val = 0;
    while (*string) {
        if (G_UNLIKELY(!is_digit(*string))) {
            *error = 1;
            return 0;
        }
        val = val * 10 + (*string++ - '0');
    }
    return val;
}

inline int parse_int_unsafe(char *string) {
    int val = 0;
    while (*string) {
        val = val * 10 + (*string++ - '0');
    }
    return val;
}

double parse_double_safe(char *string, int *error) {
    int integer_part = 0;
    double decimal_part = 0;

    while (*string) {
        if (*string == '.') {
            string++;
            break;
        }
        if (G_UNLIKELY(!is_digit(*string))) {
            *error = 1;
            return 0;
        }
        integer_part = integer_part * 10 + (*string++ - '0');
    }

    double current_decimal_part_divider = 10;

    while (*string) {
        if (G_UNLIKELY(!is_digit(*string))) {
            *error = 1;
            return 0;
        }
        decimal_part += (*string++ - '0') / current_decimal_part_divider;
        current_decimal_part_divider *= 10;
    }

    return integer_part + decimal_part;
}

int is_date_valid(Date date) {
    return date.encoded_date != 0;
}

/**
 * Returns if the given day, month and year form a valid date (1<=dd<=31 && 1<=mm<=12 && 0<=year)
 */
static int is_date(int day, int month, int year) {
    return day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 0;
}

inline Date parse_date(char *string) {
    // 18/12/2022
    if (string[2] != '/' || string[5] != '/' || string[10] != '\0') {
        return invalid_date;
    }

    string[2] = '\0';
    string[5] = '\0';
    string[10] = '\0';

    int error = 0;

    int day = parse_int_safe(string, &error);
    int month = parse_int_safe(string + 3, &error);
    int year = parse_int_safe(string + 6, &error);

    if (error) {
        return invalid_date;
    }

    if (!is_date(day, month, year)) {
        return invalid_date;
    }

    return create_date(day, month, year);
}

inline Date create_date(int day, int month, int year) {
    uint32_t encoded = 0;
    encoded |= day;
    encoded |= month << 5;
    encoded |= year << 10;
    return (Date){.encoded_date = encoded};
}

int date_get_day(Date date) {
    return (int) date.encoded_date & 0x1F;
}

int date_get_month(Date date) {
    return (int) (date.encoded_date >> 5) & 0xF;
}

int date_get_year(Date date) {
    return (int) (date.encoded_date >> 10) & 0x7FFF;
}

inline Gender parse_gender(const char *string) {
    return (string[0] == 'F' ? F : M);
}

inline CarClass parse_car_class(char *string) {
    str_to_upper(string);
    if (strcmp(string, "BASIC") == 0) {
        return BASIC;
    } else if (strcmp(string, "GREEN") == 0) {
        return GREEN;
    } else if (strcmp(string, "PREMIUM") == 0) {
        return PREMIUM;
    } else {
        return INVALID_CAR_CLASS;
    }
}

inline AccountStatus parse_acc_status(char *string) {
    str_to_upper(string);
    if (strcmp(string, "ACTIVE") == 0) {
        return ACTIVE;
    } else if (strcmp(string, "INACTIVE") == 0) {
        return INACTIVE;
    } else {
        return INVALID_ACCOUNT_STATUS;
    }
}

inline PaymentMethod parse_pay_method(const char *string) {
    return (string[1] == 'a' ? CASH : (string[1] == 'r' ? CREDIT : DEBIT));
}

int get_age(Date date_of_birth) {
    int day = date_get_day(date_of_birth);
    int month = date_get_month(date_of_birth);
    int year = date_get_year(date_of_birth);

    if (reference_date_month > month ||
        (reference_date_month == month && reference_date_day >= day))
        return reference_date_year - year;
    else
        return reference_date_year - year - 1;
}

int date_compare(Date date_1, Date date_2) {
    return (int) date_1.encoded_date - (int) date_2.encoded_date;
}
