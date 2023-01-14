#include "lazy.h"
#include "benchmark.h"

struct Lazy {
    void *value;
    ApplyFunction function_to_apply;
    gboolean function_applied;
};

Lazy *lazy_of(void *value, ApplyFunction func) {
    Lazy *lazy = malloc(sizeof(struct Lazy));
    lazy->value = value;
    lazy->function_to_apply = func;
    lazy->function_applied = FALSE;
    return lazy;
}

void *lazy_get_raw_value(Lazy *lazy) {
    return lazy->value;
}

void *lazy_get_value(Lazy *lazy) {
    BENCHMARK_START(lazy_timer);
    lazy_apply_function(lazy);
    BENCHMARK_END(lazy_timer, "     lazy_get_value: %lf\n");
    return lazy->value;
}

void lazy_apply_function(Lazy *lazy) {
    if (!lazy->function_applied) {
        if (lazy->function_to_apply != NULL) {
            lazy->function_to_apply(lazy->value);
        }
        lazy->function_applied = TRUE;
    }
}

void free_lazy(Lazy *lazy, FreeFunction free_func) {
    if (free_func != NULL) {
        free_func(lazy->value);
    }
    free(lazy);
}
