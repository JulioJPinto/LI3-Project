#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

#include "output_writer.h"

struct OutputWriter {
    void *buffer;
    void (*write)(void *, const char *, va_list);
    void (*close)(void *);
};

static void write_to_file(void *file, const char *format, va_list args) {
    vfprintf(file, format, args);
}

static void write_to_array_of_strings(void *array, const char *format, va_list args) {
    char *string = g_strdup_vprintf(format, args);
    g_ptr_array_add(array, string);
}

static void free_array_of_strings(void *array) {
    g_ptr_array_free(array, TRUE);
}

OutputWriter *create_file_output_writer(FILE *file) {
    OutputWriter *buffer = malloc(sizeof(OutputWriter));
    buffer->buffer = file;
    buffer->write = write_to_file;
    buffer->close = NULL;
    return buffer;
}

OutputWriter *create_array_of_strings_output_writer(void) {
    OutputWriter *buffer = malloc(sizeof(OutputWriter));
    buffer->buffer = g_ptr_array_new_with_free_func(free);
    buffer->write = write_to_array_of_strings;
    buffer->close = free_array_of_strings;
    return buffer;
}

void write_output_line(OutputWriter *buffer, const char *format, ...) {
    if (buffer->write != NULL) {
        va_list args;
        va_start(args, format);
        buffer->write(buffer->buffer, format, args);
        va_end(args);
    }
}

void close_output_writer(OutputWriter *buffer) {
    if (buffer->close != NULL) {
        buffer->close(buffer->buffer);
    }
    free(buffer);
}
