#pragma once
#ifndef LI3_TOKEN_ITERATOR_H
#define LI3_TOKEN_ITERATOR_H

/**
 * Abstraction of a token iterator.
 */
typedef struct TokenIterator TokenIterator;

/**
 * Creates a new TokenIterator that iterates over a line of a CSV file (semicolon separated values).
 */
TokenIterator *init_semicolon_separated_token_iterator(void);

/**
 * Returns a copy of the current line of the iterator.
 * The returned string must be freed.
 * Used to show the whole line in tests.
 */
char *token_iterator_current(TokenIterator *iterator);

/**
 * Sets the current line of the iterator without having to allocate a new iterator.
 */
void token_iterator_set_current(TokenIterator *iterator, char *string);

/**
 * Advances the iterator to the next token in the line.
 * Returns the next token.
 *
 * In the case of a CSV file, the next token is the next value separated by a semicolon
 */
char *token_iterator_next(TokenIterator *iterator);

/**
 * Frees the memory allocated for the iterator.
 */
void token_iterator_free(TokenIterator *iterator);

#endif //LI3_TOKEN_ITERATOR_H
