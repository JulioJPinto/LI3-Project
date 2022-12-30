#include "catalog/lazy.h"


typedef struct Lazy {
    void* value;
    FunctionToApply func;
    gboolean initialized;
};

Lazy* lazy_of(void* voidp, FunctionToApply func) {
     Lazy* lazy = malloc(sizeof (struct Lazy));
     lazy->value = voidp;
     lazy->func = func;
     lazy->initialized = FALSE;
     return lazy;
}

void* get_value(Lazy* lazy) {
    return lazy->value;
}

void* get_value_apply_func(Lazy* lazy) {
    if(!lazy->initialized) {
        lazy->func(lazy->value);
        lazy->initialized = TRUE;
    }
    return lazy->value;
}

void* free_lazy(Lazy* lazy) {
    free(lazy->value);
    free(lazy);
}