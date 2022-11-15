#include "user.h"
#include <glib.h>

struct User {
    char *username;
    char *name;
    Gender gender;
    Date birthdate;
    Date account_create_date;
    PaymentMethod payment_method;
    AccountStatus account_status;

    // query specific
    int rides_amount;
    int accumulated_score;
    double total_spent;
};

User *create_user(char *username, char *name, Gender gender, Date birthdate, Date acc_creation, PaymentMethod pay_method, AccountStatus acc_status) {
    User *user = malloc(sizeof(struct User));

    user->username = g_strdup(username);
    user->name = g_strdup(name);
    user->gender = gender;
    user->birthdate = birthdate;
    user->account_create_date = acc_creation;
    user->payment_method = pay_method;
    user->account_status = acc_status;

    return user;
}

void free_user(User *user) {
    free(user->username);
    free(user->name);
    free(user);
}

char *user_get_username(User *user) {
    return user->username;
}

char *user_get_name(User *user) {
    return user->name;
}

Gender user_get_gender(User *user) {
    return user->gender;
}

Date user_get_birthdate(User *user) {
    return user->birthdate;
}

AccountStatus user_get_account_status(User *user) {
    return user->account_status;
}

void user_increment_number_of_rides(User *user) {
    user->rides_amount++;
}

void user_add_score(User *user, int score) {
    user->accumulated_score += score;
}

void user_add_spent(User *user, double spent) {
    user->total_spent += spent;
}

double user_get_total_spent(User *user) {
    return user->total_spent;
}

int user_get_number_of_rides(User *user) {
    return user->rides_amount;
}

double user_get_average_score(User *user) {
    return (double) user->accumulated_score / (double) user->rides_amount;
}
