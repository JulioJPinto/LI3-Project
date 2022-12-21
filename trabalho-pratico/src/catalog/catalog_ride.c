#include "catalog/catalog_ride.h"

#include "catalog_sort.h"

struct CatalogRide {
    GPtrArray *rides_array;
    GHashTable *rides_in_city_hashtable;
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

CatalogRide *create_catalog_ride(void) {
    CatalogRide *catalog_ride = malloc(sizeof(CatalogRide));
    catalog_ride->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);
    catalog_ride->rides_in_city_hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, glib_wrapper_ptr_array_free);

    return catalog_ride;
}

void free_catalog_ride(CatalogRide *catalog_ride) {
    g_ptr_array_free(catalog_ride->rides_array, TRUE);
    g_hash_table_destroy(catalog_ride->rides_in_city_hashtable);

    free(catalog_ride);
}

static inline void catalog_ride_index_city(CatalogRide *catalog_ride, Ride *ride) {
    char *city = ride_get_city(ride); // Only needs to be freed if the city is already in the hashtable

    GPtrArray *rides_in_city = g_hash_table_lookup(catalog_ride->rides_in_city_hashtable, city);
    if (rides_in_city == NULL) {
        rides_in_city = g_ptr_array_new();
        g_hash_table_insert(catalog_ride->rides_in_city_hashtable, city, rides_in_city);
    } else {
        free(city);
    }

    g_ptr_array_add(rides_in_city, ride);
}

void catalog_ride_register_ride(CatalogRide *catalog_ride, Ride *ride) {
    g_ptr_array_add(catalog_ride->rides_array, ride);

    catalog_ride_index_city(catalog_ride, ride);
}

gboolean catalog_ride_city_has_rides(CatalogRide *catalog_ride, char *city) {
    return g_hash_table_contains(catalog_ride->rides_in_city_hashtable, city);
}

double catalog_ride_get_average_price_in_city(CatalogRide *catalog_ride, char *city) {
    GPtrArray *rides_in_city = g_hash_table_lookup(catalog_ride->rides_in_city_hashtable, city);
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

double catalog_ride_get_average_distance_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date) {
    GPtrArray *rides = catalog_ride->rides_array;

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
    GPtrArray *rides_in_city = g_hash_table_lookup(catalog_ride->rides_in_city_hashtable, city);
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

void catalog_get_passengers_that_gave_tip_in_date_range(CatalogRide *catalog_ride, Date start_date, Date end_date, GPtrArray *result) {
    GPtrArray *rides = catalog_ride->rides_array;

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

void catalog_ride_notify_stop_registering(CatalogRide *catalog_ride) {
    // Sort rides by date for queries that requires lookup in a date range
    g_ptr_array_sort(catalog_ride->rides_array, glib_wrapper_compare_rides_by_date);

    // Sort each rides array in the rides_in_city_hashtable by date for queries that requires date range in a city
    // TODO: Maybe make so the sort for each city is only done when a query for that city is called
    g_hash_table_foreach(catalog_ride->rides_in_city_hashtable, hash_table_sort_array_values_by_date, NULL);
}
