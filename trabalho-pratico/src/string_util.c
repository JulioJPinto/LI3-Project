#include "string_util.h"

#include <ctype.h>

inline char *next_token(char **line, char delim) {
    char *start = *line;

    while (**line != delim && **line != '\0') {
        (*line)++;
    }

    if (**line == delim) {
        **line = '\0';
        (*line)++;
    }

    return start;
}

void str_to_upper(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        string[i] = (char) toupper(string[i]);
    }
}
