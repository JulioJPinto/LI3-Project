#include "user.h"
#include <glib.h>

typedef struct User {
    char *username;
    char *name;
    Gender gender;
    Date birth_date;
    Date account_create_date;
    PaymentMethod payment_method;
    AccountStatus account_status;

    // query specific
    int rides_amount;
    int accumulated_score;
    double total_spent;
} User;

User *create_user(char *username, char *name, Gender gender, Date birth_date, Date acc_creation, PaymentMethod pay_method, AccountStatus acc_status) {
    User *user = malloc(sizeof(struct User));

    user->username = g_strdup(username);
    user->name = g_strdup(name);
    user->gender = gender;
    user->birth_date = birth_date;
    user->account_create_date = acc_creation;
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

void user_increment_number_of_rides(User *user) {
    user->rides_amount++;
}

void user_add_score(User *user, int score) {
    user->accumulated_score += score;
}

void user_add_spent(User *user, double spent) {
    user->total_spent += spent;
}
