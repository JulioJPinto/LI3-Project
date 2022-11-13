#include <glib.h>

#include "parser.h"
#include <stdio.h>

typedef void *(*parse_line_function_pointer)(char *string);

void read_file(FILE *csv_file, parse_line_function_pointer parse_line_function) {

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


void *parse_driver(char *line) {
    char *saveptr;

    char *idString = strtok(line, ";");
    int id = parse_int(idString);

    char *name = strtok(NULL, ";");

    char *dateString = strtok(NULL, ";");
    Date date = parse_date(dateString);

    char *genderString = strtok(NULL, ";");
    Gender gender = parse_gender(genderString);

    char *carClassString = strtok(NULL, ";");
    CarClass car_class = parse_car_class(carClassString);

    char *license_plate = strtok(NULL, ";");

    char *city = strtok(NULL, ";");

    char *creationDateString = strtok(NULL, ";");
    Date creation_date = parse_date(creationDateString);

    char *accStatusString = strtok(NULL, ";");
    AccountStatus acc_status = parse_acc_status(accStatusString);

    Driver *driver = create_driver(id, name, date, gender, car_class, license_plate, city, creation_date, acc_status);
}

void *parse_ride(char *line) {
    char *saveptr;

    char *idString = strtok(line, ";");
    int id = parse_int(idString);

    char *dateString = strtok(NULL, ";");
    Date date = parse_date(dateString);

    char *driverString = strtok(NULL, ";");
    int driver_id = parse_int(driverString);

    char *user = strtok(NULL, ";");

    char *city = strtok(NULL, ";");

    char *distanceString = strtok(NULL, ";");
    int distance = parse_int(distanceString);

    char *userScoreString = strtok(NULL, ";");
    int user_score = parse_int(userScoreString);

    char *driverScoreString = strtok(NULL, ";");
    int driver_score = parse_int(driverScoreString);

    char *tipString = strtok(NULL, ";");
    double tip = parse_double(tipString);

    char *comment = strtok(NULL, ";");

    Ride *ride = create_ride(id, date, driver_id, user, city, distance, user_score, driver_score, tip);
}

void *parse_user(char *line) {
    char *saveptr;

    char *username = strtok(line, ";");

    char *name = strtok(NULL, ";");

    char *genderString = strtok(NULL, ";");
    Gender gender = parse_gender(genderString);

    char *birthDateString = strtok(NULL, ";");
    Date birth_date = parse_date(birthDateString);

    char *accCreationString = strtok(NULL, ";");
    Date acc_creation = parse_date(accCreationString);

    char *payMethodString = strtok(NULL, ";");
    PaymentMethod pay_method = parse_pay_method(payMethodString);

    char *accStatusString = strtok(NULL, ";");
    AccountStatus acc_status = parse_acc_status(accStatusString);

    User *user = create_user(username, name, gender, birth_date, acc_creation, pay_method, acc_status);
}
