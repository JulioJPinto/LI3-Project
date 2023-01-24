#include "sort_util.h"

void sort_array(GPtrArray *array, GCompareFunc compare_func) {
    qsort(array->pdata, array->len, sizeof(gpointer), compare_func);
}
