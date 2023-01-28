#include "user.h"

#include <glib.h>
#include "string_util.h"

/**
 * Struct that represents a user.
 */
struct User {
    char *username;
    char *name;
    double total_spent;
    Date birthdate;
    Date account_create_date;
    Date most_recent_ride;
    int rides_amount;
    int accumulated_score;
    int total_distance;
    Gender gender;
    PaymentMethod payment_method;
    AccountStatus account_status;
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

    user->rides_amount = 0;
    user->accumulated_score = 0;
    user->total_spent = 0;
    user->total_distance = 0;
    user->most_recent_ride = (Date){0};

    return user;
}

User *parse_line_user(char *line, char delim) {
    char *username = next_token(&line, delim);
    if (IS_EMPTY(username)) return NULL;

    char *name = next_token(&line, delim);
    if (IS_EMPTY(name)) return NULL;

    char *gender_string = next_token(&line, delim);
    if (IS_EMPTY(gender_string)) return NULL;
    Gender gender = parse_gender(gender_string);

    char *birth_date_string = next_token(&line, delim);
    Date birth_date = parse_date(birth_date_string);
    if (!is_date_valid(birth_date)) return NULL;

    char *acc_creation_string = next_token(&line, delim);
    Date acc_creation = parse_date(acc_creation_string);
    if (!is_date_valid(acc_creation)) return NULL;

    char *pay_method_string = next_token(&line, delim);
    if (IS_EMPTY(pay_method_string)) return NULL;
    PaymentMethod pay_method = parse_pay_method(pay_method_string);

    char *acc_status_string = next_token(&line, delim);
    AccountStatus acc_status = parse_acc_status(acc_status_string);
    if (acc_status == INVALID_ACCOUNT_STATUS) return NULL;

    return create_user(username, name, gender, birth_date, acc_creation, pay_method, acc_status);
}

void free_user(User *user) {
    free(user->username);
    free(user->name);
    free(user);
}

char *user_get_username(User *user) {
    return g_strdup(user->username);
}

char *user_get_name(User *user) {
    return g_strdup(user->name);
}

Gender user_get_gender(User *user) {
    return user->gender;
}

Date user_get_birthdate(User *user) {
    return user->birthdate;
}

Date user_get_account_creation_date(User *user) {
    return user->account_create_date;
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

int user_get_total_distance(User *user) {
    return user->total_distance;
}

int user_add_total_distance(User *user, int distance) {
    return user->total_distance += distance;
}

Date user_get_most_recent_ride(User *user) {
    return user->most_recent_ride;
}

void user_register_ride_date(User *user, Date date) {
    Date user_last_ride = user->most_recent_ride;

    if (date_compare(user_last_ride, date) < 0) {
        user->most_recent_ride = date;
    }
}

int compare_users_by_total_distance(const void *a, const void *b) {
    User *a_user = *((User **) a);
    User *b_user = *((User **) b);

    int by_activeness = (int) user_get_account_status(a_user) - (int) user_get_account_status(b_user);
    if (by_activeness != 0) {
        return by_activeness;
    }

    int total_distance_a = user_get_total_distance(a_user);
    int total_distance_b = user_get_total_distance(b_user);

    int by_total_distance = total_distance_b - total_distance_a;
    if (by_total_distance != 0) {
        return by_total_distance;
    }

    Date last_ride_date_a = user_get_most_recent_ride(a_user);
    Date last_ride_date_b = user_get_most_recent_ride(b_user);

    int by_last_ride = date_compare(last_ride_date_b, last_ride_date_a);
    if (by_last_ride != 0) {
        return by_last_ride;
    }

    return strcmp(a_user->username, b_user->username);
}
