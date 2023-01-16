#pragma once
#ifndef LI3_STRING_UTIL_H
#define LI3_STRING_UTIL_H

#define IS_EMPTY(string) (string[0] == '\0')

/**
 * Similar to strtok but only accepts one character as delimiter and does not use a static buffer.
 * Advances the line pointer to the next token and sets the end of the current token to '\0'.
 * Returns a pointer to the start of the token.
 */
char *next_token(char **line, char delim);

/**
 * Makes a string uppercase.
 */
void str_to_upper(char *string);

/**
 * Makes a string lowercase.
 */
void str_to_lower(char *string);

#endif //LI3_STRING_UTIL_H
