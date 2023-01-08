#pragma once
#ifndef LI3_LAZY_H
#define LI3_LAZY_H

#include <glib.h>

/**
 * Struct that represents a lazy.
 */
typedef struct Lazy Lazy;

/**
 * Typedef that represents the function that will be applied to the lazy.
 */
typedef void (*FunctionToApply)(void *);

/**
 * Typedef that represents the function that will be applied to free lazy.
 */
typedef void (*FreeFunction)(void *);

/**
 * Struct that creates a lazy.
 * void *voidp: gptrarray
 * FunctionToApply func: array sort function
 */
Lazy *lazy_of(void *voidp, FunctionToApply func);

/**
 * Returns the raw value of the lazy.
 * The raw value is simply the value of the lazy without applying the sort function.
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
