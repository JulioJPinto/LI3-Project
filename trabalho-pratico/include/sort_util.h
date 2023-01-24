#pragma once
#ifndef LI3_SORT_UTIL_H
#define LI3_SORT_UTIL_H

#include <glib.h>

/**
 * Sorts an array of pointers given a comparison function.
 * Uses qsort from stdlib.h internally.
 */
void sort_array(GPtrArray *array, GCompareFunc compare_func);

#endif //LI3_SORT_UTIL_H
