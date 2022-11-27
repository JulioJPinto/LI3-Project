#include "string_util.h"

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
