#include <glib.h>

#include "parser.h"
#include <stdio.h>

typedef void *(*parse_line_function_pointer)(char *string);

void parse_file(FILE *csv_file, parse_line_function_pointer parse_line_function) {

    static char buffer[1024];
    for (int i = 0; fgets(buffer, 1024, csv_file); i++) {
        if (i == 0) fgets(buffer, 1024, csv_file);
        parse_line_function(buffer);
    }
}

int parse_int(char *string) {
    return strtol(string, NULL, 10);
}

double parse_double(char *string) {
    return strtod(string, NULL);
}

Date parse_date(char *string) {
    char *svptr;
    Date date_parsed = {

            .day = parse_int(strtok_r(string, "/", &svptr)),
            .month = parse_int(strtok_r(NULL, "/", &svptr)),
            .year = parse_int(strtok_r(NULL, ";", &svptr))

    };

    return date_parsed;
}

Gender parse_gender(char *string) {

    return (string[0] == 'F' ? F : M);
}

CarClass parse_car_class(char *string) {

    return (string[0] == 'g' ? GREEN : (string[0] == 'p' ? PREMIUM : BASIC));
}


AccountStatus parse_acc_status(char *string) {

    return (string[0] == 'a' ? ACTIVE : INACTIVE);
}


PaymentMethod parse_pay_method(char *string) {

    return (string[1] == 'a' ? CASH : (string[1] == 'r' ? CREDIT : DEBIT));
}


void *parse_driver_line(char *line) {
    char *saveptr;
    const char delim = ';';

    char *idString = strtok(line, delim);
    int id = parse_int(idString);

    char *name = strtok(NULL, delim);

    char *dateString = strtok(NULL, delim);
    Date date = parse_date(dateString);

    char *genderString = strtok(NULL, delim);
    Gender gender = parse_gender(genderString);

    char *carClassString = strtok(NULL, delim);
    CarClass car_class = parse_car_class(carClassString);

    char *license_plate = strtok(NULL, delim);

    char *city = strtok(NULL, delim);

    char *creationDateString = strtok(NULL, delim);
    Date creation_date = parse_date(creationDateString);

    char *accStatusString = strtok(NULL, delim);
    AccountStatus acc_status = parse_acc_status(accStatusString);

    Driver *driver = create_driver(id, name, date, gender, car_class, license_plate, city, creation_date, acc_status);
}

void *parse_ride_line(char *line) {
    char *saveptr;
    const char delim = ';';

    char *idString = strtok(line, delim);
    int id = parse_int(idString);

    char *dateString = strtok(NULL, delim);
    Date date = parse_date(dateString);

    char *driverString = strtok(NULL, delim);
    int driver_id = parse_int(driverString);

    char *user = strtok(NULL, delim);

    char *city = strtok(NULL, delim);

    char *distanceString = strtok(NULL, delim);
    int distance = parse_int(distanceString);

    char *userScoreString = strtok(NULL, delim);
    int user_score = parse_int(userScoreString);

    char *driverScoreString = strtok(NULL, delim);
    int driver_score = parse_int(driverScoreString);

    char *tipString = strtok(NULL, delim);
    double tip = parse_double(tipString);

    char *comment = strtok(NULL, delim);

    Ride *ride = create_ride(id, date, driver_id, user, city, distance, user_score, driver_score, tip);
}

void *parse_user_line(char *line) {
    char *saveptr;
    const char delim = ';';

    char *username = strtok(line, delim);

    char *name = strtok(NULL, delim);

    char *genderString = strtok(NULL, delim);
    Gender gender = parse_gender(genderString);

    char *birthDateString = strtok(NULL, delim);
    Date birth_date = parse_date(birthDateString);

    char *accCreationString = strtok(NULL, delim);
    Date acc_creation = parse_date(accCreationString);

    char *payMethodString = strtok(NULL, delim);
    PaymentMethod pay_method = parse_pay_method(payMethodString);

    char *accStatusString = strtok(NULL, delim);
    AccountStatus acc_status = parse_acc_status(accStatusString);

    User *user = create_user(username, name, gender, birth_date, acc_creation, pay_method, acc_status);
}
