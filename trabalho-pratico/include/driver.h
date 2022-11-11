#pragma once
#ifndef LI3_DRIVER_H
#define LI3_DRIVER_H

#include "struct_util.h"
#include <glib.h>

typedef struct driver Driver;

Driver *create_driver(int id, char* name, Date birth_date, Gender gender, CarClass car_class, char* license_plate,
                      char* city, Date account_creation_date, AccountStatus account_status);

int driver_get_id(Driver *driver);

void free_driver(Driver *driver);

#endif //LI3_DRIVER_H
