#include <glib.h>

#include "driver.h"
#include "struct_util.h"

struct Driver {
    int id;
    char *name;
    Date birth_date;
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
};

Driver *create_driver(int id, char *name, Date birth_date, Gender gender, CarClass car_class, char *license_plate,
                      char *city, Date account_creation_date, AccountStatus account_status) {
    Driver *driver = malloc(sizeof(Driver));
    driver->id = id;
    driver->name = name;
    driver->birth_date = birth_date;
    driver->gender = gender;
    driver->car_class = car_class;
    driver->license_plate = license_plate;
    driver->city = city;
    driver->account_creation_date = account_creation_date;
    driver->account_status = account_status;

    return driver;
}

int driver_get_id(Driver *driver) {
    return driver->id;
}

CarClass driver_get_car_class(Driver *driver) {
    return driver->car_class;
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

void free_driver(Driver *driver) {
    free(driver);
}
