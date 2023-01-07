#pragma once
#ifndef LI3_LAZY_H
#define LI3_LAZY_H

#include <glib.h>

typedef struct Lazy Lazy;

typedef void (*FunctionToApply)(void *);

typedef void (*FreeFunction)(void *);

Lazy *lazy_of(void *voidp, FunctionToApply func);

void *lazy_get_raw_value(Lazy *lazy);

void *lazy_get_value(Lazy *lazy);

void *free_lazy(Lazy *lazy, FreeFunction free_func);

#endif //LI3_LAZY_H
