#include "catalog/catalog_ride.h"

<<<<<<< HEAD
#include "benchmark.h"
=======
#include "catalog/lazy.h"
>>>>>>> 4fe6623 (refactor: change all ptrarray to lazys)
#include "catalog_sort.h"
#include "ride_driver_and_user_info.h"

struct CatalogRide {
    Lazy *rides_lazy;
    GHashTable *rides_in_city_hashtable;
    GHashTable *rides_by_id_hashtable;

    Lazy *rduinfo_male_lazy;
    Lazy *rduinfo_female_lazy;
};

/**
 * Function that wraps free ride to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_ride(gpointer ride) {
    free_ride(ride);
}

/**
 * Function that wraps free array to be used in GLib g_hash_table free func.
 */
void glib_wrapper_ptr_array_free(gpointer array) {
    g_ptr_array_free(array, TRUE);
}

void sort_array_rides_by_date(void *rides_array) {
    GPtrArray *rides_ptr_array = *(GPtrArray **) rides_array;
    sort_array(rides_ptr_array, compare_rides_by_date);
}

void sort_rduinfo_by_account_creation_date(void *rides_array) {
    GPtrArray *rides_ptr_array = *(GPtrArray **) rides_array;
    sort_array(rides_ptr_array, compare_rduinfo_by_account_creation_date);

}

CatalogRide *create_catalog_ride(void) {
    CatalogRide *catalog_ride = malloc(sizeof(CatalogRide));
<<<<<<< HEAD
    catalog_ride->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);
    catalog_ride->rides_by_id_hashtable = g_hash_table_new(g_direct_hash, g_direct_equal);
=======
    catalog_ride->rides_lazy = lazy_of(g_ptr_array_new_with_free_func(glib_wrapper_free_ride), sort_array_rides_by_date);
>>>>>>> 4fe6623 (refactor: change all ptrarray to lazys)
    catalog_ride->rides_in_city_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, glib_wrapper_ptr_array_free);

    catalog_ride->rduinfo_male_lazy = lazy_of(g_ptr_array_new_with_free_func(free_rduinfo), sort_rduinfo_by_account_creation_date);
    catalog_ride->rduinfo_female_lazy = lazy_of(g_ptr_array_new_with_free_func(free_rduinfo), sort_rduinfo_by_account_creation_date);

    return catalog_ride;
}

void free_catalog_ride(CatalogRide *catalog_ride) {
<<<<<<< HEAD
    g_ptr_array_free(catalog_ride->rides_array, TRUE);
    g_hash_table_destroy(catalog_ride->rides_by_id_hashtable);
=======
    free_lazy(catalog_ride->rides_lazy);
>>>>>>> 4fe6623 (refactor: change all ptrarray to lazys)
    g_hash_table_destroy(catalog_ride->rides_in_city_hashtable);

    free_lazy(catalog_ride->rduinfo_male_lazy);
    free_lazy(catalog_ride->rduinfo_female_lazy);

    free(catalog_ride);
}

static inline void catalog_ride_index_city(CatalogRide *catalog_ride, Ride *ride) {
    char *city = ride_get_city(ride); // Only needs to be freed if the city is already in the hashtable

    Lazy *rides_in_city = g_hash_table_lookup(catalog_ride->rides_in_city_hashtable, city);
    if (rides_in_city == NULL) {
        rides_in_city = lazy_of(g_ptr_array_new(), sort_array_rides_by_date);
        g_hash_table_insert(catalog_ride->rides_in_city_hashtable, city, rides_in_city);
    } else {
        free(city);
    }
    GPtrArray* rides_in_city_array = *(GPtrArray **) get_value(rides_in_city);

    g_ptr_array_add(rides_in_city_array, ride);
}

void catalog_ride_register_ride(CatalogRide *catalog_ride, Ride *ride) {
    GPtrArray* rides_in_city_array = *(GPtrArray **) get_value(catalog_ride->rides_lazy);
    g_ptr_array_add(rides_in_city_array, ride);

    g_hash_table_insert(catalog_ride->rides_by_id_hashtable, GINT_TO_POINTER(ride_get_id(ride)), ride);
    catalog_ride_index_city(catalog_ride, ride);
}

void catalog_ride_register_rduinfo_same_gender(CatalogRide *catalog_ride,
                                               Gender gender,
                                               RideDriverAndUserInfo *rduinfo) {
    g_ptr_array_add(gender == M ? *(GPtrArray **) get_value(catalog_ride->rduinfo_male_lazy) : *(GPtrArray **) get_value(catalog_ride->rduinfo_female_lazy),
                    rduinfo);
}

Ride *catalog_ride_get_ride(CatalogRide *catalog_ride, int ride_id) {
    return g_hash_table_lookup(catalog_ride->rides_by_id_hashtable, GINT_TO_POINTER(ride_id));
}

gboolean catalog_ride_city_has_rides(CatalogRide *catalog_ride, char *city) {
    return g_hash_table_contains(catalog_ride->rides_in_city_hashtable, city);
}

double catalog_ride_get_average_price_in_city(CatalogRide *catalog_ride, char *city) {
    GPtrArray *rides_in_city = *(GPtrArray **) get_value(g_hash_table_lookup(catalog_ride->rides_in_city_hashtable, city));
    if (rides_in_city == NULL) return 0;

    double price_sum = 0;

    // Performance impact of this loop is negligible, even with ~200000 rides_in_city per city.
    // We also assume that the user won't ask this query twice for the same city, so no need to cache the average.

    for (guint i = 0; i < rides_in_city->len; i++) {
        Ride *ride = g_ptr_array_index(rides_in_city, i);
        price_sum += ride_get_price(ride);
    }

    return price_sum / rides_in_city->len;
}

/**
 * Returns the index of the lowest ride whose date is greater than the given date.
 */
static guint ride_array_find_date_lower_bound(GPtrArray *array, Date date) {
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

double catalog_ride_get_average_distance_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date) {
    GPtrArray *rides = *(GPtrArray **) get_value(catalog_ride->rides_lazy);

    long current_value_index = ride_array_find_date_lower_bound(rides, start_date);

    double price_sum = 0;
    int rides_count = 0;

    while (current_value_index < rides->len) {
        Ride *current_ride = g_ptr_array_index(rides, current_value_index);
        Date current_ride_date = ride_get_date(current_ride);

        if (date_compare(current_ride_date, end_date) > 0)
            break;

        price_sum += ride_get_price(current_ride);
        rides_count++;

        current_value_index++;
    }

    // divide by zero check
    return rides_count != 0 ? price_sum / rides_count : -1;
}

double catalog_ride_get_average_distance_in_city_and_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, char *city) {
    GPtrArray *rides_in_city = *(GPtrArray **) get_value(g_hash_table_lookup(catalog_ride->rides_in_city_hashtable, city));
    if (rides_in_city == NULL) return 0;

    long current_value_index = ride_array_find_date_lower_bound(rides_in_city, start_date);

    double distance_sum = 0;
    int rides_count = 0;

    while (current_value_index < rides_in_city->len) {
        Ride *current_ride = g_ptr_array_index(rides_in_city, current_value_index);
        Date current_ride_date = ride_get_date(current_ride);

        if (date_compare(current_ride_date, end_date) > 0)
            break;

        distance_sum += ride_get_distance(current_ride);
        rides_count++;

        current_value_index++;
    }

    if (rides_count == 0) return -1;

    return distance_sum / rides_count;
}

void catalog_ride_get_passengers_that_gave_tip_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, GPtrArray *result) {
    GPtrArray *rides = *(GPtrArray **) get_value(catalog_ride->rides_lazy);

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

    sort_array(result, compare_rides_by_distance);
}

int catalog_ride_get_rides_with_user_and_driver_with_same_age_above_acc_age(CatalogRide *catalog_ride, GPtrArray *result, Gender gender, int min_account_age) {
    GPtrArray *rduinfo_array = gender == M ? *(GPtrArray **) get_value(catalog_ride->rduinfo_male_lazy) : *(GPtrArray **) get_value(catalog_ride->rduinfo_female_lazy);

    int i = 0;
    while (i < (int) rduinfo_array->len) {
        RideDriverAndUserInfo *rduinfo = g_ptr_array_index(rduinfo_array, i);

        Date user_account_creation_date = rduinfo_get_user_account_creation_date(rduinfo);
        Date driver_account_creation_date = rduinfo_get_driver_account_creation_date(rduinfo);

        int user_age = get_age(user_account_creation_date);
        int driver_age = get_age(driver_account_creation_date);

        if (user_age >= min_account_age && driver_age >= min_account_age) {
            g_ptr_array_add(result, rduinfo);
        }

        if (driver_age < min_account_age) {
            // Since the array is sorted by driver birthdate, we can skip all the rides with driver_age < min_account_age
            break;
        }

        i++;
    }
    return i;
}

void catalog_ride_notify_stop_registering(CatalogRide *catalog_ride) {
    BENCHMARK_START(sort_rides_array_timer);
    // Sort rides by date for queries that requires lookup in a date range
<<<<<<< HEAD
    sort_array(catalog_ride->rides_array, compare_rides_by_date);
    BENCHMARK_END(sort_rides_array_timer, "     sort_rides_array_by_date: %lf seconds\n");
=======
    GPtrArray *rides_array = *(GPtrArray **) get_value(catalog_ride->rides_lazy);
    sort_array(rides_array, compare_rides_by_date);
>>>>>>> 4fe6623 (refactor: change all ptrarray to lazys)

    // Sort each rides array in the rides_in_city_hashtable by date for queries that requires date range in a city
    // TODO: Maybe make so the sort for each city is only done when a query for that city is called

    BENCHMARK_START(sort_rides_in_city_array_timer);
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, catalog_ride->rides_in_city_hashtable);
<<<<<<< HEAD
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        g_timer_start(sort_rides_in_city_array_timer);
        sort_array(value, compare_rides_by_date);
        log_info("     sort_rides_in_city_array (%s): %lf seconds\n", key, g_timer_elapsed(sort_rides_in_city_array_timer, NULL));
    }

    BENCHMARK_START(sort_rduinfo_male_array_timer);
    sort_array(catalog_ride->rduinfo_male_array, compare_rduinfo_by_account_creation_date);
    BENCHMARK_END(sort_rduinfo_male_array_timer, "     sort_rduinfo_male_array_timer: %lf seconds\n");
    BENCHMARK_START(sort_rduinfo_female_array_timer);
    sort_array(catalog_ride->rduinfo_female_array, compare_rduinfo_by_account_creation_date);
    BENCHMARK_END(sort_rduinfo_female_array_timer, "     sort_rduinfo_female_array_timer: %lf seconds\n");
=======
    while (g_hash_table_iter_next(&iter, NULL, &value)) {
        GPtrArray *value_array = *(GPtrArray **) get_value(value);
        sort_array(value_array, compare_rides_by_date);
    }

    GPtrArray *rduinfo_male_array = *(GPtrArray **) catalog_ride->rduinfo_male_lazy;
    sort_array(rduinfo_male_array, compare_rduinfo_by_account_creation_date);
    GPtrArray *rduinfo_female_array = *(GPtrArray **) catalog_ride->rduinfo_female_lazy;
    sort_array(rduinfo_female_array, compare_rduinfo_by_account_creation_date);
>>>>>>> 4fe6623 (refactor: change all ptrarray to lazys)
}
