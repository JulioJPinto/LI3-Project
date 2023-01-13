#pragma once
#ifndef LI3_LAZY_H
#define LI3_LAZY_H

#include <glib.h>
/**
 * In this file we defined everything needed for a Lazy to work properly.
 * Starting with the lazy itself, the typedef for the functions applied to the lazy,
 * the functions to get the value of the lazy, the raw value or the value after applying
 * the function, the function that creates a lazy and the function that frees it.
 */

/**
 * Struct that represents a lazy.
 */
typedef struct Lazy Lazy;

/**
 * Typedef that represents the function that will be applied to the lazy.
 */
typedef void (*FunctionToApply)(void *);

/**
 * Typedef that represents the function that frees the lazy.
 */
typedef FunctionToApply FreeFunction;

/**
 * Function that creates a lazy.
 * void *voidp : value
 * FunctionToApply func: function to apply in value
 */
Lazy *lazy_of(void *voidp, FunctionToApply func);

/**
 * Returns the lazy's value without applying any function
 */
void *lazy_get_raw_value(Lazy *lazy);

/**
 * Returns the value of the lazy after the sort function is applied.
 */
void *lazy_get_value(Lazy *lazy);

/**
 * Frees the memory allocated for the Lazy.
 */
void *free_lazy(Lazy *lazy, FreeFunction free_func);

#endif //LI3_LAZY_H
