#pragma once
#ifndef LI3_USER_H
#define LI3_USER_H

#include <glib.h>
#include "struct_util.h"

typedef struct user User;

User *create_user(char *username, char *name, Gender gender, Date birth_date, Date acc_creation, PaymentMethod pay_method, AccountStatus acc_status);

/*
 * Returns the username of the user
 */
char *user_get_username(User *user);

#endif //LI3_USER_H
