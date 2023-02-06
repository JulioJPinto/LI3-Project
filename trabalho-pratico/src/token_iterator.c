#include "token_iterator.h"

#include "string_util.h"
#include <stdlib.h>
#include <glib.h>

/**
 * Struct implementation of TokenIterator.
 */
struct TokenIterator {
    char *string;
};

TokenIterator *init_semicolon_separated_token_iterator(void) {
    TokenIterator *iterator = malloc(sizeof(TokenIterator));
    return iterator;
}

char *token_iterator_current(TokenIterator *iterator) {
    return g_strdup(iterator->string);
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
