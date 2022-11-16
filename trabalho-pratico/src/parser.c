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

void *parse_line_driver(char *line) {
    char *id_string = strtok(line, ";");
    int id = parse_int(id_string);

    char *name = strtok(NULL, ";");

    char *date_string = strtok(NULL, ";");
    Date date = parse_date(date_string);

    char *gender_string = strtok(NULL, ";");
    Gender gender = parse_gender(gender_string);

    char *car_class_string = strtok(NULL, ";");
    CarClass car_class = parse_car_class(car_class_string);

    char *license_plate = strtok(NULL, ";");

    char *city = strtok(NULL, ";");

    char *creation_date_string = strtok(NULL, ";");
    Date creation_date = parse_date(creation_date_string);

    char *acc_status_string = strtok(NULL, ";");
    AccountStatus acc_status = parse_acc_status(acc_status_string);

    return create_driver(id, name, date, gender, car_class, license_plate, city, creation_date, acc_status);
}

void *parse_line_ride(char *line) {
    char *id_string = strtok(line, ";");
    int id = parse_int(id_string);

    char *date_string = strtok(NULL, ";");
    Date date = parse_date(date_string);

    char *driver_string = strtok(NULL, ";");
    int driver_id = parse_int(driver_string);

    char *user = strtok(NULL, ";");

    char *city = strtok(NULL, ";");

    char *distance_string = strtok(NULL, ";");
    int distance = parse_int(distance_string);

    char *user_score_string = strtok(NULL, ";");
    int user_score = parse_int(user_score_string);

    char *driver_score_string = strtok(NULL, ";");
    int driver_score = parse_int(driver_score_string);

    char *tip_string = strtok(NULL, ";");
    double tip = parse_double(tip_string);

    //    char *comment = strtok(NULL, ";");

    return create_ride(id, date, driver_id, user, city, distance, user_score, driver_score, tip);
}

void *parse_line_user(char *line) {
    char *username = strtok(line, ";");

    char *name = strtok(NULL, ";");

    char *gender_string = strtok(NULL, ";");
    Gender gender = parse_gender(gender_string);

    char *birth_date_string = strtok(NULL, ";");
    Date birth_date = parse_date(birth_date_string);

    char *acc_creation_string = strtok(NULL, ";");
    Date acc_creation = parse_date(acc_creation_string);

    char *pay_method_string = strtok(NULL, ";");
    PaymentMethod pay_method = parse_pay_method(pay_method_string);

    char *acc_status_string = strtok(NULL, ";");
    AccountStatus acc_status = parse_acc_status(acc_status_string);

    return create_user(username, name, gender, birth_date, acc_creation, pay_method, acc_status);
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
