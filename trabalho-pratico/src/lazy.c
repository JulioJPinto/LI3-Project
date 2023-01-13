#include "lazy.h"
#include "benchmark.h"

typedef struct Lazy {
    void *value;
    FunctionToApply func;
    gboolean initialized;
};

Lazy *lazy_of(void *voidp, FunctionToApply func) {
    Lazy *lazy = malloc(sizeof(struct Lazy));
    lazy->value = voidp;
    lazy->func = func;
    lazy->initialized = FALSE;
    return lazy;
}

void *lazy_get_raw_value(Lazy *lazy) {
    return lazy->value;
}

void *lazy_get_value(Lazy *lazy) {
    BENCHMARK_START(lazy_timer);
    if (!lazy->initialized) {

        lazy->func(lazy->value);
        lazy->initialized = TRUE;
    }
    BENCHMARK_END(lazy_timer, "     lazy_get_value: %lf\n");
    return lazy->value;
}

void *free_lazy(Lazy *lazy, FreeFunction free_func) {
    free_func(lazy->value);
    free(lazy);
}
