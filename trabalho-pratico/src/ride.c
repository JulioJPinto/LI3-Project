#include <glib.h>

#include "ride.h"
#include "struct_util.h"

typedef struct ride {
    int id;
    Date date;
    int driver_id;
    GString user_username;
    GString city;
    int distance;
    int score_user;
    int score_driver;
    float tip;
    // GString comment;
} Ride;

GString *ride_get_city(Ride *ride) {
    return g_string_new_len(ride->city.str, (gssize) ride->city.len);
}
