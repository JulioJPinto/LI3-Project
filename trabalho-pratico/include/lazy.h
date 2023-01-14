#pragma once
#ifndef LI3_LAZY_H
#define LI3_LAZY_H

#include <glib.h>

/**
 * This file implements a lazy evaluation system.
 * A value and a function to apply to it are stored in the Lazy struct.
 *
 * The function is only applied once when the value is requested and
 * sequent calls to lazy_get_value will return the value without applying the function again.
 *
 * This is useful for cases where the function to apply to the value
 * is expensive and may not be needed at that moment or at all.
 *
 * Lazies are used in the program to only sort some arrays when they are needed,
 * thus reducing the loading time of the program.
 */

/**
 * Struct that represents a lazy.
 */
typedef struct Lazy Lazy;

/**
 * Typedef that represents the function that will be applied to the lazy.
 */
typedef void (*ApplyFunction)(void *);

/**
 * Typedef that represents the function that frees the lazy.
 */
typedef ApplyFunction FreeFunction;

/**
 * Function that creates a lazy.
 * @param value The value to store in the lazy.
 * @param apply_function The function to apply to the value when it is requested.
 */
Lazy *lazy_of(void *value, ApplyFunction func);

/**
 * Returns the value saved in the lazy struct without executing the apply function.
 */
void *lazy_get_raw_value(Lazy *lazy);

/**
 * Returns the value saved in the lazy struct after executing the apply function.
 * The apply function is only executed once and sequent calls to this function will
 * return the value without executing the apply function again.
 */
void *lazy_get_value(Lazy *lazy);

/**
 * Applies the apply function to the value in the lazy struct.
 * The apply function is only executed once.
 */
void lazy_apply_function(Lazy *lazy);

/**
 * Frees the memory allocated for the Lazy.
 * The value should be freed by the free function.
 * The free function can be NULL if the value doesn't need to be freed.
 */
void free_lazy(Lazy *lazy, FreeFunction free_func);

#endif //LI3_LAZY_H
