#include "catalog/catalog_ride.h"

#include "array_util.h"
#include "benchmark.h"
#include "lazy.h"

/**
 * Struct that holds all the rides and indexed information.
 */
struct CatalogRide {
    Lazy *lazy_rides_array;
    GPtrArray *array_of_rides_in_city_array;

    Lazy *lazy_ride_male_array;
    Lazy *lazy_ride_female_array;
};

/**
 * Function that wraps free ride to be used in GLib g_ptr_array free func.
 */
void glib_wrapper_free_ride(gpointer ride) {
    free_ride(ride);
}

/**
 * Function that sorts the rides array by date.
 */
static void sort_rides_array(gpointer rides_array) {
    BENCHMARK_START(sort_rides_array_timer);
    sort_array(rides_array, compare_rides_by_date);
    BENCHMARK_END(sort_rides_array_timer, "sort_rides_array: %lf seconds\n");
}

/**
 * Function that sorts the male rides array by driver and user account creation date.
 */
static void sort_male_rides_by_account_creation_date(gpointer male_rides_array) {
    BENCHMARK_START(sort_rduinfo_male_array_timer);
    sort_array(male_rides_array, compare_ride_by_driver_and_user_account_creation_date);
    BENCHMARK_END(sort_rduinfo_male_array_timer, "sort_ride_male_array: %lf seconds\n");
}

/**
 * Function that sorts the female rides array by driver and user account creation date.
 */
void sort_female_rides_by_account_creation_date(gpointer female_rides_array) {
    BENCHMARK_START(sort_rduinfo_female_array_timer);
    sort_array(female_rides_array, compare_ride_by_driver_and_user_account_creation_date);
    BENCHMARK_END(sort_rduinfo_female_array_timer, "sort_ride_female_array: %lf seconds\n");
}

/**
 * Frees the rides array.
 */
void free_rides_array(gpointer array) {
    g_ptr_array_free(array, TRUE);
}

/**
 * Function that wraps free array to be used in GLib g_hash_table free func.
 */
void free_lazy_with_rides_array(gpointer lazy) {
    // Lazy can be NULL if the city has no rides registered.
    if (lazy == NULL) return;

    free_lazy(lazy, free_rides_array);
}

CatalogRide *create_catalog_ride(void) {
    CatalogRide *catalog_ride = malloc(sizeof(CatalogRide));
    catalog_ride->lazy_rides_array = lazy_of(g_ptr_array_new_with_free_func(glib_wrapper_free_ride), sort_rides_array);
    catalog_ride->array_of_rides_in_city_array = g_ptr_array_new_with_free_func(free_lazy_with_rides_array);

    catalog_ride->lazy_ride_male_array = lazy_of(g_ptr_array_new(), sort_male_rides_by_account_creation_date);
    catalog_ride->lazy_ride_female_array = lazy_of(g_ptr_array_new(), sort_female_rides_by_account_creation_date);

    return catalog_ride;
}

void free_catalog_ride(CatalogRide *catalog_ride) {
    free_lazy(catalog_ride->lazy_rides_array, free_rides_array);
    g_ptr_array_free(catalog_ride->array_of_rides_in_city_array, TRUE);

    free_lazy(catalog_ride->lazy_ride_male_array, free_rides_array);
    free_lazy(catalog_ride->lazy_ride_female_array, free_rides_array);

    free(catalog_ride);
}

/**
 * Function that sorts the rides in city array by date.
 */
static void sort_array_rides_in_city_array(gpointer rides_array) {
    BENCHMARK_START(sort_rides_array_timer);
    sort_array(rides_array, compare_rides_by_date);
    BENCHMARK_END(sort_rides_array_timer, "sort_rides_in_city_array: %lf seconds\n");
}

/**
 * Returns a Lazy with an array of rides in a city.
 */
static Lazy *catalog_ride_get_rides_in_city(CatalogRide *catalog_ride, int city_id) {
    return g_ptr_array_get_at_index_safe(catalog_ride->array_of_rides_in_city_array, city_id);
}

/**
 * Indexes a ride by city id. 
 * If the city has no rides registered, it creates a new array and adds the ride to it.
 * If the city already has rides registered, it adds the ride to the array.
 */
static inline void catalog_ride_index_city(CatalogRide *catalog_ride, Ride *ride, int city_id) {
    Lazy *rides_in_city = catalog_ride_get_rides_in_city(catalog_ride, city_id);
    if (rides_in_city == NULL) {
        rides_in_city = lazy_of(g_ptr_array_new(), sort_array_rides_in_city_array);

        g_ptr_array_set_at_index_safe(catalog_ride->array_of_rides_in_city_array, city_id, rides_in_city);
    }

    g_ptr_array_add(lazy_get_raw_value(rides_in_city), ride);
}

void catalog_ride_register_ride(CatalogRide *catalog_ride, Ride *ride) {
    g_ptr_array_add(lazy_get_raw_value(catalog_ride->lazy_rides_array), ride);

    catalog_ride_index_city(catalog_ride, ride, ride_get_city_id(ride));
}

void catalog_ride_register_ride_same_gender(CatalogRide *catalog_ride,
                                            Gender gender,
                                            Ride *ride) {
    GPtrArray *ride_same_gender_array = lazy_get_raw_value(gender == M ? catalog_ride->lazy_ride_male_array : catalog_ride->lazy_ride_female_array);
    g_ptr_array_add(ride_same_gender_array, ride);
}

double catalog_ride_get_average_price_in_city(CatalogRide *catalog_ride, int city_id) {
    GPtrArray *rides_in_city = lazy_get_value(catalog_ride_get_rides_in_city(catalog_ride, city_id));
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
    GPtrArray *rides = lazy_get_value(catalog_ride->lazy_rides_array);

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

double catalog_ride_get_average_distance_in_city_and_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, int city_id) {
    GPtrArray *rides_in_city = lazy_get_value(catalog_ride_get_rides_in_city(catalog_ride, city_id));
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
    GPtrArray *rides = lazy_get_value(catalog_ride->lazy_rides_array);

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
    GPtrArray *ride_same_gender_array = lazy_get_value(gender == M ? catalog_ride->lazy_ride_male_array : catalog_ride->lazy_ride_female_array);

    int i = 0;
    while (i < (int) ride_same_gender_array->len) {
        Ride *ride = g_ptr_array_index(ride_same_gender_array, i);

        Date user_account_creation_date = ride_get_user_account_creation_date(ride);
        Date driver_account_creation_date = ride_get_driver_account_creation_date(ride);

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

void catalog_ride_force_eager_indexing(CatalogRide *catalog_ride) {
    lazy_apply_function(catalog_ride->lazy_rides_array);

    // Sort each rides array in the array_of_rides_in_city_array by date for queries that requires date range in a city

    for (int i = 0; i < (int) catalog_ride->array_of_rides_in_city_array->len; ++i) {
        BENCHMARK_LOG("(%d) ", i);
        gpointer lazy = catalog_ride->array_of_rides_in_city_array->pdata[i];
        if (lazy != NULL) lazy_apply_function(lazy);
    }

    lazy_apply_function(catalog_ride->lazy_ride_male_array);
    lazy_apply_function(catalog_ride->lazy_ride_female_array);
}
