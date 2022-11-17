#include <string.h>
#include <glib.h>

#include "file_util.h"
#include "logger.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#define OUTPUT_FOLDER "Resultados"

FILE *open_file_folder(const char *const folder_path, const char *const file_name) {
    gchar *string = g_build_path(PATH_SEPARATOR, folder_path, file_name, NULL);
    FILE *file = open_file(string);

    free(string);

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

void create_output_folder_if_not_exists(void) {
    g_mkdir_with_parents(OUTPUT_FOLDER, 0777);
}

FILE *create_command_output_file(int command_number) {
    char *filename = g_strdup_printf(OUTPUT_FOLDER PATH_SEPARATOR "command%d_output.txt", command_number);
    FILE *output = fopen(filename, "w");
    g_free(filename);
    return output;
}
