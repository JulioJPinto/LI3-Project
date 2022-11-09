#pragma once
#ifndef LI3_USER_H
#define LI3_USER_H

#include <glib.h>

typedef struct user User;

/*
 * Returns the username of the user
 */
char *user_get_username(User *user);

#endif //LI3_USER_H
