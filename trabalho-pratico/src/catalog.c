#include "catalog.h"

#include "price_util.h"
#include "catalog_sort.h"

/**
 * Struct that represents a catalog.
 */
struct Catalog {
    GPtrArray *users_array;
    GPtrArray *drivers_array;
    GPtrArray *rides_array;

    GHashTable *user_from_username_hashtable; // key: username (char*), value: User*
    GHashTable *driver_from_id_hashtable;     // key: driver id (int), value: Driver*

    GHashTable *rides_in_city_hashtable;               // key: city (char*), value: GPtrArray of rides
    GHashTable *driver_city_info_collection_hashtable; // key: city (char*), value: DriverCityInfoCollection
    //GHashTable <City, Struct<GHashTable<driver_id, DriverCityInfo>, GPtrArray<DriverCityInfo>>

    GPtrArray *rides_with_gender_male;
    GPtrArray *rides_with_gender_female;
};

/**
 * Function that wraps free user to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_user(gpointer user) {
    free_user(user);
}

/**
 * Function that wraps free driver to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_driver(gpointer driver) {
    free_driver(driver);
}

/**
 * Function that wraps free ride to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_ride(gpointer ride) {
    free_ride(ride);
}

/**
 * Function that wraps free array to be used in GLib g_hash_table free func.
 */
void glib_wrapper_ptr_array_free_segment(gpointer array) {
    g_ptr_array_free(array, TRUE);
}

typedef struct {
    /**
     * Array of all DriverCityInfo structs for the city.
     * This will be sorted by the driver's score once all the rides are registered.
     * This is used to get the best drivers in O(1).
     */
    GPtrArray *driver_city_info_array;
    /**
     * Hash table that maps driver ids to DriverCityInfo structs.
     * This is used to get the DriverCityInfo of a driver id in O(1) for faster insertions.
     * This hashtable is only valid during the insertion of rides, and is freed after the insertion.
     * You can't use this hashtable after the insertion ends (when `notify_stop_registering` is called).
     */
    GHashTable *driver_city_info_hashtable;
} DriverCityInfoCollection;

void free_driver_city_info_collection(DriverCityInfoCollection *collection) {
    // We don't need to free the hashtable because it is already freed in `notify_stop_registering` (in the end of the rides insertions).
    g_ptr_array_free(collection->driver_city_info_array, TRUE);

    free(collection);
}

void glib_wrapper_free_driver_city_info_collection(gpointer collection) {
    free_driver_city_info_collection(collection);
}

Catalog *create_catalog(void) {
    Catalog *catalog = malloc(sizeof(struct Catalog));

    catalog->users_array = g_ptr_array_new_full(100000, glib_wrapper_free_user);
    catalog->drivers_array = g_ptr_array_new_full(10000, glib_wrapper_free_driver);
    catalog->rides_array = g_ptr_array_new_full(1000000, glib_wrapper_free_ride);

    catalog->user_from_username_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    catalog->driver_from_id_hashtable = g_hash_table_new_full(g_int_hash, g_int_equal, free, NULL);

    catalog->rides_in_city_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, glib_wrapper_ptr_array_free_segment);
    catalog->driver_city_info_collection_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, glib_wrapper_free_driver_city_info_collection);

    catalog->rides_with_gender_male = g_ptr_array_new();
    catalog->rides_with_gender_female = g_ptr_array_new();

    return catalog;
}

void free_catalog(Catalog *catalog) {
    g_ptr_array_free(catalog->users_array, TRUE);
    g_ptr_array_free(catalog->drivers_array, TRUE);
    g_ptr_array_free(catalog->rides_array, TRUE);

    g_hash_table_destroy(catalog->user_from_username_hashtable);
    g_hash_table_destroy(catalog->driver_from_id_hashtable);
    g_hash_table_destroy(catalog->rides_in_city_hashtable);
    g_hash_table_destroy(catalog->driver_city_info_collection_hashtable);

    g_ptr_array_free(catalog->rides_with_gender_male, TRUE);
    g_ptr_array_free(catalog->rides_with_gender_female, TRUE);

    free(catalog);
}

static inline void internal_parse_and_register_user(Catalog *catalog, char *line, char separator) {
    User *user = parse_line_user(line, separator);

    g_ptr_array_add(catalog->users_array, user);

    char *key = user_get_username(user);
    // No need to free the key, it's freed by the hashtable when the user is removed

    g_hash_table_insert(catalog->user_from_username_hashtable, key, user);
}

void parse_and_register_user(void *catalog, char *line, char separator) {
    internal_parse_and_register_user(catalog, line, separator);
}

static inline void internal_parse_and_register_driver(Catalog *catalog, char *line, char separator) {
    Driver *driver = parse_line_driver(line, separator);

    g_ptr_array_add(catalog->drivers_array, driver);

    int *key = malloc(sizeof(int));
    *key = driver_get_id(driver);
    // No need to free the key, it will be freed when the Driver is freed

    g_hash_table_insert(catalog->driver_from_id_hashtable, key, driver);
}

void parse_and_register_driver(void *catalog, char *line, char separator) {
    internal_parse_and_register_driver(catalog, line, separator);
}

/**
 * Internal function of internal_parse_and_register_ride that indexes the city of the ride.
 */
static void query_6_catalog_ride_index_city(Catalog *catalog, Ride *ride) {
    char *city = ride_get_city(ride); // Only needs to be freed if the city is already in the hashtable

    GPtrArray *rides_in_city;
    if (g_hash_table_contains(catalog->rides_in_city_hashtable, city)) {
        rides_in_city = g_hash_table_lookup(catalog->rides_in_city_hashtable, city);
        free(city);
    } else {
        rides_in_city = g_ptr_array_new();
        g_hash_table_insert(catalog->rides_in_city_hashtable, city, rides_in_city);
    }

    g_ptr_array_add(rides_in_city, ride);
}


static void query_7_catalog_driver_city_info_register(Catalog *catalog, int driver_id, char *driver_name, int driver_score, char *ride_city) {
    DriverCityInfoCollection *driver_city_collection = g_hash_table_lookup(catalog->driver_city_info_collection_hashtable, ride_city);

    DriverCityInfo *target;
    if (driver_city_collection == NULL) { // ride_city is not in the hashtable
        driver_city_collection = malloc(sizeof(DriverCityInfoCollection));
        driver_city_collection->driver_city_info_array = g_ptr_array_new_with_free_func(free_driver_city_info_voidp);
        driver_city_collection->driver_city_info_hashtable = g_hash_table_new_full(g_int_hash, g_int_equal, free, NULL);

        g_hash_table_insert(catalog->driver_city_info_collection_hashtable, g_strdup(ride_city), driver_city_collection);
        goto register_driver_city_info; // We can skip the lookup because we know it's not in the hashtable
    }

    target = g_hash_table_lookup(driver_city_collection->driver_city_info_hashtable, &driver_id);

    if (target == NULL) { // driver is not yet registered in city
    register_driver_city_info:
        target = create_driver_city_info(driver_id, driver_name);

        g_ptr_array_add(driver_city_collection->driver_city_info_array, target);
        int *key = malloc(sizeof(int));
        *key = driver_id;
        g_hash_table_insert(driver_city_collection->driver_city_info_hashtable, key, target);
    }

    driver_city_info_register_ride_score(target, driver_score);
}

void query_8_catalog_ride_index_by_gender(Catalog *catalog, Ride *ride, User *user, Driver *driver) {
    Gender user_gender = user_get_gender(user);
    Gender driver_gender = driver_get_gender(driver);
    if (user_gender != driver_gender) return;

    GPtrArray *rides_with_gender_array = user_gender == M ? catalog->rides_with_gender_male : catalog->rides_with_gender_female;
    g_ptr_array_add(rides_with_gender_array, ride);
}

static inline void internal_parse_and_register_ride(Catalog *catalog, char *line, char separator) {
    Ride *ride = parse_line_ride(line, separator);

    g_ptr_array_add(catalog->rides_array, ride);

    int driver_id = ride_get_driver_id(ride);
    Driver *driver = catalog_get_driver(catalog, driver_id);
    double price = compute_price(ride_get_distance(ride), driver_get_car_class(driver));
    ride_set_price(ride, price);

    double total_price = ride_get_tip(ride) + price;

    driver_increment_number_of_rides(driver);
    int driver_score = ride_get_score_driver(ride);
    driver_add_score(driver, driver_score);
    driver_add_earned(driver, total_price);
    driver_register_ride_date(driver, ride_get_date(ride));

    char *username = ride_get_user_username(ride);

    User *user = catalog_get_user(catalog, username);
    user_increment_number_of_rides(user);
    user_add_score(user, ride_get_score_user(ride));
    user_add_spent(user, total_price);
    user_add_total_distance(user, ride_get_distance(ride));
    user_register_ride_date(user, ride_get_date(ride));

    free(username);

    query_6_catalog_ride_index_city(catalog, ride);

    AccountStatus driver_account_status = driver_get_account_status(driver);
    AccountStatus user_account_status = user_get_account_status(user);

    if (driver_account_status == ACTIVE) { // We only need to index for query 7 if the driver is active
        char *city = ride_get_city(ride);
        char *driver_name = driver_get_name(driver);
        query_7_catalog_driver_city_info_register(catalog, driver_id, driver_name, driver_score, city);
        free(driver_name);
        free(city);
    }

    if (driver_account_status == ACTIVE && user_account_status == ACTIVE) { // We only need to index for query 8 if both driver and user is active
        query_8_catalog_ride_index_by_gender(catalog, ride, user, driver);
    }
}

void parse_and_register_ride(void *catalog, char *line, char separator) {
    internal_parse_and_register_ride(catalog, line, separator);
}

User *catalog_get_user(Catalog *catalog, char *username) {
    return g_hash_table_lookup(catalog->user_from_username_hashtable, username);
}

Driver *catalog_get_driver(Catalog *catalog, int id) {
    return g_hash_table_lookup(catalog->driver_from_id_hashtable, &id);
}

gboolean catalog_city_exists(Catalog *catalog, char *city) {
    return g_hash_table_contains(catalog->rides_in_city_hashtable, city);
}

int query_2_catalog_get_top_drivers_with_best_score(Catalog *catalog, int n, GPtrArray *result) {
    // TODO: todo in query_3_catalog_get_top_users_with_longest_total_distance
    int length = MIN(n, (int) catalog->drivers_array->len);

    for (int i = 0; i < length; i++) {
        g_ptr_array_add(result, g_ptr_array_index(catalog->drivers_array, i));
    }

    return length;
}

int query_3_catalog_get_top_users_with_longest_total_distance(Catalog *catalog, int n, GPtrArray *result) {
    // TODO: This can be improved, no need to copy the array, just create a new one pointing to the users_array with the length of n
    int length = MIN(n, (int) catalog->users_array->len);

    for (int i = 0; i < n; i++) {
        g_ptr_array_add(result, g_ptr_array_index(catalog->users_array, i));
    }

    return length;
}


double query_4_catalog_get_average_price_in_city(Catalog *catalog, char *city) {
    GPtrArray *rides = g_hash_table_lookup(catalog->rides_in_city_hashtable, city);
    if (rides == NULL) return 0;

    double total_price = 0;

    // Performance impact of this loop is negligible, even with ~200000 rides per city.
    // We also assume that the user won't ask this query twice for the same city, so no need to cache.

    for (guint i = 0; i < rides->len; i++) {
        Ride *ride = g_ptr_array_index(rides, i);
        total_price += ride_get_price(ride);
    }

    return total_price / rides->len;
}

/**
 * Returns the index of the lowest ride whose date is greater than the given date.
 */
guint ride_array_find_date_lower_bound(GPtrArray *array, Date date) {
    guint mid;

    guint low = 0;
    guint high = array->len;

    while (low < high) {
        mid = low + (high - low) / 2;

        if (date_compare(date, ride_get_date(g_ptr_array_index(array, mid))) <= 0) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    // TODO: maybe stop binary search when finds a date that is equal to
    //  the given date and then do a linear search backwards to find the lower bound

    return low;
}

/*
 * This can also be done with a hashtable that keeps the accumulated price for each date,
 * do two binary searches for the lower and upper bounds and return the difference.
 * (maybe not worth it)
 */
double query_5_catalog_get_average_price_in_date_range(Catalog *catalog, Date start_date, Date end_date) {
    GPtrArray *rides = catalog->rides_array;

    long current_value_index = ride_array_find_date_lower_bound(rides, start_date);

    double total_price = 0;
    int rides_count = 0;

    while (current_value_index < rides->len) {
        Ride *current_ride = g_ptr_array_index(rides, current_value_index);
        Date current_ride_date = ride_get_date(current_ride);

        if (date_compare(current_ride_date, end_date) > 0)
            break;

        total_price += ride_get_price(current_ride);
        rides_count++;

        current_value_index++;
    }

    // divide by zero check
    return rides_count != 0 ? total_price / rides_count : -1;
}

double query_6_catalog_get_average_distance_in_city_by_date(Catalog *catalog, Date start_date, Date end_date, char *city) {
    GPtrArray *rides_in_city = g_hash_table_lookup(catalog->rides_in_city_hashtable, city);
    if (rides_in_city == NULL) return 0;

    double total_distance = 0;
    int ride_count = 0;

    long current_value_index = ride_array_find_date_lower_bound(rides_in_city, start_date);

    while (current_value_index < rides_in_city->len) {
        Ride *current_ride = g_ptr_array_index(rides_in_city, current_value_index);
        Date current_ride_date = ride_get_date(current_ride);

        if (date_compare(current_ride_date, end_date) > 0)
            break;

        total_distance += ride_get_distance(current_ride);
        ride_count++;

        current_value_index++;
    }

    // divide by zero check
    return ride_count != 0 ? total_distance / ride_count : -1;
}

int query_7_catalog_get_top_n_drivers_in_city(Catalog *catalog, int n, char *city, GPtrArray *result) {
    DriverCityInfoCollection *driver_city_info_collection = g_hash_table_lookup(catalog->driver_city_info_collection_hashtable, city);
    if (driver_city_info_collection == NULL) return 0; // city doesn't exist

    GPtrArray *top_drivers_in_city = driver_city_info_collection->driver_city_info_array;

    int size = MIN(n, (int) top_drivers_in_city->len);

    for (int i = 0; i < size; i++) {
        DriverCityInfo *driver = g_ptr_array_index(top_drivers_in_city, i);
        g_ptr_array_add(result, driver);
    }

    return size;
}

int query_8_catalog_get_rides_with_user_and_driver_with_same_gender_above_acc_min_age(Catalog *catalog, GPtrArray *result, Gender gender, int min_account_age) {
    GPtrArray *rides_with_gender = gender == M ? catalog->rides_with_gender_male : catalog->rides_with_gender_female;

    int i = 0;
    while (i < (int) rides_with_gender->len) {
        Ride *ride = g_ptr_array_index(rides_with_gender, i);

        char *user_username = ride_get_user_username(ride);
        int driver_id = ride_get_driver_id(ride);

        User *user = catalog_get_user(catalog, user_username);
        Driver *driver = catalog_get_driver(catalog, driver_id);

        free(user_username);

        Date user_account_creation_date = user_get_account_creation_date(user);
        Date driver_account_creation_date = driver_get_account_creation_date(driver);

        int user_age = get_age(user_account_creation_date);
        int driver_age = get_age(driver_account_creation_date);

        if (user_age >= min_account_age && driver_age >= min_account_age) {
            g_ptr_array_add(result, ride);
        }

        if (driver_age < min_account_age) {
            // Since the array is sorted by driver birthdate, we can skip all the rides with driver_age < min_account_age
            break;
        }

        i++;
    }
    return i;
}

void query_9_catalog_get_passengers_that_gave_tip_in_date_range(Catalog *catalog, GPtrArray *result, Date start_date, Date end_date) {
    GPtrArray *rides = catalog->rides_array;

    long current_value_index = ride_array_find_date_lower_bound(rides, start_date);

    Ride *current_ride;
    Date current_ride_date;

    while (current_value_index < rides->len) {
        current_ride = g_ptr_array_index(rides, current_value_index);
        current_ride_date = ride_get_date(current_ride);

        if (date_compare(current_ride_date, end_date) > 0)
            break;

        if (ride_get_tip(current_ride) > 0) {
            g_ptr_array_add(result, current_ride);
        }

        current_value_index++;
    }

    g_ptr_array_sort(result, glib_wrapper_compare_rides_by_distance);
}

/**
 * Sorts the value by date.
 * This is used to sort a hash table with value: array of rides.
 * @param key unused
 * @param value the array to be sorted
 * @param user_data unused
 */
void hash_table_sort_array_values_by_date(gpointer key, gpointer value, gpointer user_data) {
    (void) key;
    (void) user_data;

    g_ptr_array_sort(value, glib_wrapper_compare_rides_by_date);
}

void hash_table_prepare_driver_city_info_collection_for_queries(gpointer key, gpointer value, gpointer user_data) {
    (void) key;
    (void) user_data;

    DriverCityInfoCollection *driver_city_collection = value;
    // We don't need the hashtable anymore, so we can free it
    g_hash_table_destroy(driver_city_collection->driver_city_info_hashtable);

    // Sort the array by average score
    g_ptr_array_sort(driver_city_collection->driver_city_info_array, glib_wrapper_compare_driver_city_infos_by_average_score);
}

void notify_stop_registering(Catalog *catalog) {
    g_ptr_array_sort(catalog->drivers_array, glib_wrapper_compare_drivers_by_score);
    g_ptr_array_sort(catalog->users_array, glib_wrapper_compare_users_by_total_distance);
    // Sort rides by date for faster query 5 that requires lookup in a date range
    g_ptr_array_sort(catalog->rides_array, glib_wrapper_compare_rides_by_date);

    // Sort each rides array in the rides_in_city_hashtable by date for faster query 6 that requires date range
    // TODO: Maybe make so the sort for each city is only done when a query for that city is called
    g_hash_table_foreach(catalog->rides_in_city_hashtable, hash_table_sort_array_values_by_date, NULL);

    g_ptr_array_sort_with_data(catalog->rides_with_gender_male, glib_wrapper_compare_ride_by_driver_and_user_account_creation_date, catalog);
    g_ptr_array_sort_with_data(catalog->rides_with_gender_female, glib_wrapper_compare_ride_by_driver_and_user_account_creation_date, catalog);

    g_hash_table_foreach(catalog->driver_city_info_collection_hashtable, hash_table_prepare_driver_city_info_collection_for_queries, NULL);
}
