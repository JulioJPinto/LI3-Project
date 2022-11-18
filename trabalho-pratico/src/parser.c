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

void read_file(FILE *stream, parser_line_function* parse_line_function, void* arg0, register_function* register_function) {
    char *line_buffer = malloc(1024 * sizeof(char));

    if (fgets(line_buffer, 1024, stream) == NULL) {
        return;
    }

    register_function(arg0, (parse_line_function(line_buffer)));
    
}
