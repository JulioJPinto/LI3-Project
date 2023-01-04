#include "catalog/lazy.h"


typedef struct Lazy {
    void* value;
    FunctionToApply func;
    FreeFunction free_func;
    gboolean initialized;
};

void lazy_g_ptr_free_function(gpointer array) {
    g_ptr_array_free(array, TRUE);
}

static const FreeFunction free_functions[] = {
    lazy_g_ptr_free_function
};

Lazy* lazy_of(void* voidp, FunctionToApply func, int index_func) {
     Lazy* lazy = malloc(sizeof (struct Lazy));
     lazy->value = voidp;
     lazy->func = func;
     lazy->free_func = free_functions[index_func];
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
    lazy->free_func(lazy->value);
    free(lazy);
}