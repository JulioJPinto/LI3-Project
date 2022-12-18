#include "struct_util.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const Date invalid_date = {.day = -1};
static const Date reference_date = {9, 10, 2022};

int parse_int_safe(char *string, int *error) {
    char *end;
    int result = (int) strtol(string, &end, 10);
    if (*end != '\0') {
        *error = 1;
    }
    return result;
}

inline int parse_int_unsafe(char *string) {
    return (int) strtol(string, NULL, 10);
}

double parse_double_safe(char *string, int *error) {
    char *end;
    double result = strtod(string, &end);
    if (*end != '\0') {
        *error = 1;
    }
    return result;
}

inline int is_date_valid(Date date) {
    return date.day >= 1 && date.day <= 31 && date.month >= 1 && date.month <= 12;
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

    return (Date){day, month, year};
}

inline Gender parse_gender(const char *string) {
    return (string[0] == 'F' ? F : M);
}

void to_upper(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        string[i] = (char) toupper(string[i]);
    }
}

inline CarClass parse_car_class(char *string) {
    to_upper(string);
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
    to_upper(string);
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
    if (reference_date.month > date_of_birth.month ||
        (reference_date.month == date_of_birth.month && reference_date.day >= date_of_birth.day))
        return reference_date.year - date_of_birth.year;
    else
        return reference_date.year - date_of_birth.year - 1;
}

int date_compare(Date date_1, Date date_2) {
    if (date_1.year != date_2.year)
        return date_1.year - date_2.year;
    else if (date_1.month != date_2.month)
        return date_1.month - date_2.month;
    else if (date_1.day != date_2.day)
        return date_1.day - date_2.day;
    else
        return 0;
}
