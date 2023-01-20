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

OutputWriter *create_null_output_writer(void) {
    OutputWriter *buffer = malloc(sizeof(OutputWriter));
    buffer->buffer = NULL;
    buffer->write = NULL;
    buffer->close = NULL;
    return buffer;
}

void write_output_line(OutputWriter *output_writer, const char *format, ...) {
    if (output_writer->write != NULL) {
        va_list args;
        va_start(args, format);
        output_writer->write(output_writer->buffer, format, args);
        va_end(args);
    }
}

void *get_buffer(OutputWriter *output_writer) {
    return output_writer->buffer;
}

void close_output_writer(OutputWriter *output_writer) {
    if (output_writer->close != NULL) {
        output_writer->close(output_writer->buffer);
    }
    free(output_writer);
}
