#pragma once
#ifndef LI3_ARRAY_UTIL_H
#define LI3_ARRAY_UTIL_H

#include <glib.h>

/**
 * Sorts an array of pointers given a comparison function.
 * Uses qsort from stdlib.h internally.
 */
void sort_array(GPtrArray *array, GCompareFunc compare_func);

/**
 * Sets the element at the given index to the given data.
 * If the index is greater than the array's length, the array is resized to fit the index.
 */
void g_ptr_array_set_at_index_safe(GPtrArray *array, int index, gpointer data);

/**
 * Gets the element at the given index.
 * If the index is greater than the array's length or less than 0, NULL is returned.
 */
void* g_ptr_array_get_at_index_safe(GPtrArray *array, int index);

#endif //LI3_ARRAY_UTIL_H
