#pragma once
#ifndef LI3_LAZY_H
#define LI3_LAZY_H

#include <glib.h>

typedef struct Lazy Lazy;

typedef void (*FunctionToApply)(void *);

Lazy* lazy_of(void* voidp, FunctionToApply func);

void* get_value(Lazy* lazy);

void* get_value_apply_func(Lazy* lazy)

void* free_lazy(Lazy* lazy);

#endif //LI3_LAZY_H


