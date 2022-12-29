#pragma once
#ifndef LI3_CATALOG_SORT_H
#define LI3_CATALOG_SORT_H

#include <glib.h>

/**
 * Sorts an array of pointers given a comparison function.
 * Uses qsort from stdlib.h internally.
 */
void sort_array(GPtrArray *array, GCompareFunc compare_func);

/**
 * Function that compares drivers by activeness, score, last ride and id.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the drivers array for fast resolution of the query 2.
 */
int compare_drivers_by_score(gconstpointer a, gconstpointer b);

/**
 * Function that compares users by activeness, total distance, last ride and username.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the users array for fast resolution of the query 3.
 */
int compare_users_by_total_distance(gconstpointer a, gconstpointer b);

/**
 * Function that compares rides by activeness, total distance, last ride and username.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the rides array for fast resolution of the query 5 and 6.
 */
int compare_rides_by_date(gconstpointer a, gconstpointer b);

/**
 * Function that compares DriverCityInfos by average score and id.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the DriverCityInfos array for fast resolution of the query 7.
 */
int compare_driver_city_infos_by_average_score(gconstpointer a, gconstpointer b);

/**
 * Function that compares RideDriverAndUserInfos by driver account creation date, user account creation date and then ride id.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the rides array for fast resolution of the query 8.
 */
int compare_rduinfo_by_account_creation_date(gconstpointer a, gconstpointer b);

/**
 * Function that compares rides by total distance, date, and then id.
 * This function receives gconstpointers to be used as comparison functions.
 * Used to sort the rides array for fast resolution of the query 9.
 */
int compare_rides_by_distance(gconstpointer a, gconstpointer b);

#endif //LI3_CATALOG_SORT_H
