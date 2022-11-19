#include <glib.h>

#include "driver.h"
#include "struct_util.h"
#include "parser.h"

struct Driver {
    int id;
    char *name;
    Date birthdate;
    Gender gender;
    CarClass car_class;
    char *license_plate;
    char *city;
    Date account_creation_date;
    AccountStatus account_status;

    // query specific
    int rides_amount;
    int accumulated_score;
    double total_earned;
    Date last_ride_date;
};

Driver *create_driver(int id, char *name, Date birth_date, Gender gender, CarClass car_class, char *license_plate,
                      char *city, Date account_creation_date, AccountStatus account_status) {
    Driver *driver = malloc(sizeof(Driver));
    driver->id = id;
    driver->name = g_strdup(name);
    driver->birthdate = birth_date;
    driver->gender = gender;
    driver->car_class = car_class;
    driver->license_plate = license_plate;
    driver->city = g_strdup(city);
    driver->account_creation_date = account_creation_date;
    driver->account_status = account_status;

    driver->rides_amount = 0;
    driver->accumulated_score = 0;
    driver->total_earned = 0;
    driver->last_ride_date = (Date){0, 0, 0};

    return driver;
}

void *parse_line_driver(char **arg) {

    char *id_string = arg[0];
    int id = parse_int(id_string);

    char *name = arg[1];

    char *date_string = arg[2];
    Date date = parse_date(date_string);

    char *gender_string = arg[3];
    Gender gender = parse_gender(gender_string);

    char *car_class_string = arg[4];
    CarClass car_class = parse_car_class(car_class_string);

    char *license_plate = arg[5];

    char *city = arg[6];

    char *creation_date_string = arg[7];
    Date creation_date = parse_date(creation_date_string);

    char *acc_status_string = arg[8];
    AccountStatus acc_status = parse_acc_status(acc_status_string);

    return create_driver(id, name, date, gender, car_class, license_plate, city, creation_date, acc_status);
}

void* wrapper_voidp_parse_driver(char **arg, void* driver){
    driver = parse_line_driver(arg);
}

int driver_get_id(Driver *driver) {
    return driver->id;
}

char *driver_get_name(Driver *driver) {
    return driver->name;
}

Date driver_get_birthdate(Driver *driver) {
    return driver->birthdate;
}

Gender driver_get_gender(Driver *driver) {
    return driver->gender;
}

CarClass driver_get_car_class(Driver *driver) {
    return driver->car_class;
}

AccountStatus driver_get_account_status(Driver *driver) {
    return driver->account_status;
}

Date driver_get_last_ride_date(Driver *driver) {
    return driver->last_ride_date;
}

void driver_increment_number_of_rides(Driver *driver) {
    driver->rides_amount++;
}

void driver_add_score(Driver *driver, int score) {
    driver->accumulated_score += score;
}

void driver_add_earned(Driver *driver, double earned) {
    driver->total_earned += earned;
}

int driver_get_number_of_rides(Driver *driver) {
    return driver->rides_amount;
}

double driver_get_average_score(Driver *driver) {
    return (double) driver->accumulated_score / (double) driver->rides_amount;
}

double driver_get_total_earned(Driver *driver) {
    return driver->total_earned;
}

void driver_register_last_ride_date(Driver *driver, Date date) {
    if (date_compare(date, driver->last_ride_date) > 0) {
        driver->last_ride_date = date;
    }
}

void free_driver(Driver *driver) {
    free(driver->name);
    free(driver->city);
    free(driver);
}
