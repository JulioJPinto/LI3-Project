#include "string_util.h"

#include <ctype.h>
#include <string.h>

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

void str_to_lower(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        string[i] = (char) tolower(string[i]);
    }
}

int string_ends_with(char *string, char *suffix) {
    int string_lenght = (int) strlen(string);
    int suffix_lenght = (int) strlen(suffix);

    int lenght_dif = string_lenght - suffix_lenght;

    return string_lenght >= suffix_lenght && strcmp(string + lenght_dif, suffix) == 0;
}
