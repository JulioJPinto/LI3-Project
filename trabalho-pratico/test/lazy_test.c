#include "../include/lazy.h"

#include <glib.h>

/**
 * Ensures `lazy_get_raw_value` and `lazy_get_value` work as expected when the apply function is NULL.
 */
void test_lazy_behavior_null_apply_function(void) {
    int value = 5;
    Lazy *lazy = lazy_of(&value, NULL);
    {
        int new_value = *(int *) lazy_get_raw_value(lazy);
        if (*(int *) lazy_get_raw_value(lazy) != 5) {
            g_test_fail_printf("Expected %d in lazy_get_raw_value, got %d", 5, new_value);
        }
    }
    {
        int new_value = *(int *) lazy_get_value(lazy);
        if (*(int *) lazy_get_value(lazy) != 5) {
            g_test_fail_printf("Expected %d in lazy_get_value, got %d", 5, new_value);
        }
    }

    free_lazy(lazy, NULL);
}

/**
 * Increases the value inside of an int pointer by 1.
*/
void increase_int(void *value) {
    (*(int *) value)++;
}

/**
 * Ensures `lazy_get_raw_value` and `lazy_get_value` work as expected when the apply function is not NULL.
 */
void test_lazy_behavior_int_apply_function(void) {
    int value = 5;
    Lazy *lazy = lazy_of(&value, increase_int);
    {
        int new_value = *(int *) lazy_get_raw_value(lazy);
        if (*(int *) lazy_get_raw_value(lazy) != 5) {
            g_test_fail_printf("Expected %d in lazy_get_raw_value, got %d", 5, new_value);
        }
    }
    {
        int new_value = *(int *) lazy_get_value(lazy);
        if (*(int *) lazy_get_value(lazy) != 6) {
            g_test_fail_printf("Expected %d in lazy_get_value, got %d", 6, new_value);
        }
    }
    {
        int new_value = *(int *) lazy_get_value(lazy);
        if (*(int *) lazy_get_value(lazy) != 6) {
            g_test_fail_printf("Expected %d in lazy_get_value, got %d", 6, new_value);
        }
    }

    free_lazy(lazy, NULL);
}
