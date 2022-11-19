#pragma once
#ifndef LI3_CATALOG_SORT_H
#define LI3_CATALOG_SORT_H

#include <glib.h>

int glib_wrapper_compare_drivers_by_score(gconstpointer a, gconstpointer b);

int glib_wrapper_compare_users_by_total_distance(gconstpointer a, gconstpointer b);

int glib_wrapper_compare_rides_by_date(gconstpointer a, gconstpointer b);

#endif //LI3_CATALOG_SORT_H
