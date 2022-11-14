#include "user.h"
#include <glib.h>

typedef struct user {
    char *username;
    char *name;
    Gender gender;
    Date birth_date;
    Date account_create_date;
    PaymentMethod payment_method;
    AccountStatus account_status;
} User;

User *create_user(char *username, char *name, Gender gender, Date birth_date, Date acc_creation, PaymentMethod pay_method, AccountStatus acc_status) {
    User *user = malloc(sizeof(struct user));

    user->username = username;
    user->name = name;
    user->gender = gender;
    user->birth_date = birth_date;
    user->account_create_date = acc_creation;
    user->account_status = acc_status;

    return user;
}

void free_user(User *user) {
    free(user);
}

char *user_get_username(User *user) {
    return user->username;
}
