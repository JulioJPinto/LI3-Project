#pragma once
#ifndef LI3_DRIVER_H
#define LI3_DRIVER_H

#include "struct_util.h"

typedef struct driver Driver;

Driver *create_driver(int id, GString name, Date birth_date, Gender gender, CarClass car_class, GString license_plate,
                      GString city, Date account_creation_date, AccountStatus account_status);

int driver_get_id(Driver *driver);

void free_driver(Driver *driver);

#endif //LI3_DRIVER_H
