#include <string.h>
#include <glib.h>

#include "file_util.h"
#include "logger.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

FILE *open_file_folder(const char *const folder_path, const char *const file_name) {
    GString *file_path = g_string_sized_new(strlen(folder_path) + strlen(file_name) + 1);

    g_string_append(file_path, folder_path);
    g_string_append(file_path, PATH_SEPARATOR);
    g_string_append(file_path, file_name);

    FILE *file = open_file(file_path->str);

    g_string_free(file_path, TRUE);

    return file;
}

FILE *open_file(const char *const file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        log_panic("Could not open file %s\n", file_name);
    }

    return file;
}

void format_fgets_input_line(char *line_buffer) {
    size_t line_buffer_length = strlen(line_buffer);
    if (line_buffer[line_buffer_length - 1] == '\n') {
        line_buffer[line_buffer_length - 1] = '\0';
    }
}
