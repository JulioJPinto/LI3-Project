#include "token_iterator.h"

#include "string_util.h"
#include <stdlib.h>

struct TokenIterator {
    char *string;
};

TokenIterator *init_csv_line_token_iterator(void) {
    TokenIterator *iterator = malloc(sizeof(TokenIterator));
    return iterator;
}

char *token_iterator_current(TokenIterator *iterator) {
    return iterator->string;
}

void token_iterator_set_current(TokenIterator *iterator, char *string) {
    iterator->string = string;
}

char *token_iterator_next(TokenIterator *iterator) {
    // This can be expanded to support other delimiters and input formats.
    return next_token(&iterator->string, ';');
}

void token_iterator_free(TokenIterator *iterator) {
    free(iterator);
}
