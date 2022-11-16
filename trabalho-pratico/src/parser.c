#include "parser.h"

#include <glib.h>
#include <stdio.h>

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

User *read_users_file(FILE *stream, char *line_buffer, int line_buffer_size) {
    if (fgets(line_buffer, line_buffer_size, stream) == NULL) {
        return NULL;
    }
    return parse_line_user(line_buffer);
}

Driver *read_drivers_file(FILE *stream, char *line_buffer, int line_buffer_size) {
    if (fgets(line_buffer, line_buffer_size, stream) == NULL) {
        return NULL;
    }
    return parse_line_driver(line_buffer);
}

Ride *read_rides_file(FILE *stream, char *line_buffer, int line_buffer_size) {
    if (fgets(line_buffer, line_buffer_size, stream) == NULL) {
        return NULL;
    }
    return parse_line_ride(line_buffer);
}
