#pragma once
#ifndef LI3_USER_H
#define LI3_USER_H

#include <glib.h>
#include "struct_util.h"

typedef struct User User;

User *create_user(char *username, char *name, Gender gender, Date birthdate, Date acc_creation, PaymentMethod pay_method, AccountStatus acc_status);

/*
 * Frees the memory allocated for the User.
 */
void free_user(User *user);

/*
 * Returns a copy of the username of the User
 * The caller is responsible for freeing the memory allocated for the username
 */
char *user_get_username(User *user);

/*
 * Returns a copy of the name of the User
 * The caller is responsible for freeing the memory allocated for the name
 */
char *user_get_name(User *user);

/*
 * Returns the gender of the User
 */
Gender user_get_gender(User *user);

/*
 * Returns the birthdate of the User
 */
Date user_get_birthdate(User *user);

/*
 * Returns the account status of the User
 */
AccountStatus user_get_account_status(User *user);

/*
 * Increments the rides amount of the User
 */
void user_increment_number_of_rides(User *user);

/*
 * Increments the accumulated score of the User
 */
void user_add_score(User *user, int score);

/*
 * Increments the total money spent of the User
 */
void user_add_spent(User *user, double spent);

/*
 * Returns the total money spent of the User
 */
double user_get_total_spent(User *user);

/*
 * Returns the number of rides of the User
 */
int user_get_number_of_rides(User *user);

/*
 * Returns the average score of the User
 */
double user_get_average_score(User *user);

/*
 * Return the total distance traveled by the User
 */
int user_get_total_distance(User *user);

/*
 * Increments the total distance traveled by the User
 */
int user_add_total_distance(User *user, int distance);

/*
 * Return the date of the most recent ride done by the User
 */
Date user_get_most_recent_ride(User *user);

/*
 * Sets the date of the last ride done by the user to the date passed 
 * if the date is more recent than the current set date
 */
void user_register_ride_date(User *user, Date date);

/*
 * Parses a string of the User File. 
 */
User *parse_line_user(char *line);

#endif //LI3_USER_H
