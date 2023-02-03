#include <stdlib.h>

#include "line_iterator.h"
#include "string_util.h"

struct LineIterator {
    char *string;
};

LineIterator *init_csv_line_iterator(char *string) {
    LineIterator *iterator = malloc(sizeof(LineIterator));
    iterator->string = string;
    return iterator;
}

char *line_iterator_current(LineIterator *iterator) {
    return iterator->string;
}

char *line_iterator_next(LineIterator *iterator) {
    // This can be expanded to support other delimiters and input formats.
    return next_token(&iterator->string, ';');
}

void line_iterator_free(LineIterator *iterator) {
    free(iterator);
}
