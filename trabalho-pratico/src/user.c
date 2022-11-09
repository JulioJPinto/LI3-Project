#include "user.h"

#include <glib.h>
#include "struct_util.h"

typedef struct user {
    char *username;
    char *name;
    Gender gender;
    Date birth_date;
    Date account_create_date;
    PaymentMethod payment_method;
    AccountStatus account_status;
} User;


char *user_get_username(User *user) {
    return user->username;
}
