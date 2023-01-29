#include "array_util.h"

void sort_array(GPtrArray *array, GCompareFunc compare_func) {
    qsort(array->pdata, array->len, sizeof(gpointer), compare_func);
}

void g_ptr_array_set_at_index_safe(GPtrArray *array, int index, gpointer data) {
    g_assert(index >= 0);

    if (index >= (int) array->len) {
        g_ptr_array_set_size(array, index + 1); // Reserve space for index
    }

    array->pdata[index] = data;
}

void *g_ptr_array_get_at_index_safe(GPtrArray *array, int index) {
    if (index >= (int) array->len || index < 0) {
        return NULL;
    }
    return array->pdata[index];
}
