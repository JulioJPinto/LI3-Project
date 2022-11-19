#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "struct_util.h"

static const Date reference_date = {9, 10, 2022};

int parse_int(char *string) {
    return (int) strtol(string, NULL, 10);
}

double parse_double(char *string) {
    return strtod(string, NULL);
}

Date parse_date(char *string) {
    if (strlen(string) != 10) {
        return (Date){0, 0, 0};
    }

    string[2] = '\0';
    string[5] = '\0';
    string[10] = '\0';

    int day = parse_int(string);
    int month = parse_int(string + 3);
    int year = parse_int(string + 6);

    return (Date){day, month, year};
}

Gender parse_gender(const char *string) {
    return (string[0] == 'F' ? F : M);
}

CarClass parse_car_class(const char *string) {
    return (string[0] == 'g' ? GREEN : (string[0] == 'p' ? PREMIUM : BASIC));
}

AccountStatus parse_acc_status(const char *string) {
    return (string[0] == 'a' ? ACTIVE : INACTIVE);
}

PaymentMethod parse_pay_method(const char *string) {
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
