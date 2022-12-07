#pragma once
#ifndef LI3_CATALOG_SORT_H
#define LI3_CATALOG_SORT_H

#include <glib.h>

/**
 * Function that compares drivers by activeness, score, last ride and id.
 * This function is abstracted in gconstpointer's to be used in glib sort functions.
 * Used to sort the drivers array for fast resolution of the query 2.
 */
int glib_wrapper_compare_drivers_by_score(gconstpointer a, gconstpointer b);

/**
 * Function that compares users by activeness, total distance, last ride and username.
 * This function is abstracted in gconstpointer's to be used in glib sort functions.
 * Used to sort the users array for fast resolution of the query 3.
 */
int glib_wrapper_compare_users_by_total_distance(gconstpointer a, gconstpointer b);

/**
 * Function that compares rides by activeness, total distance, last ride and username.
 * This function is abstracted in gconstpointer's to be used in glib sort functions.
 * Used to sort the rides array for fast resolution of the query 5.
 */
int glib_wrapper_compare_rides_by_date(gconstpointer a, gconstpointer b);

int glib_wrapper_compare_rides_by_distance(gconstpointer a, gconstpointer b);
#endif //LI3_CATALOG_SORT_H
