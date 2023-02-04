#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

#include "output_writer.h"

/**
 * Struct that represents a buffer.
 */
typedef struct {
    char *value;
    size_t buffer_size;
    size_t buffer_capacity;
} Buffer;

/**
 * Initializes a new buffer.
 */
Buffer init_buffer(void) {
    Buffer buffer;
    buffer.value = NULL;
    buffer.buffer_size = 0;
    buffer.buffer_capacity = 0;
    return buffer;
}

/**
 * Resets the buffer.
 */
void reset_buffer(Buffer *buffer) {
    if (buffer->value) {
        buffer->value[0] = '\0';
        buffer->buffer_size = 0;
    }
}

/**
 * Appends the given string to the buffer.
 */
void append_to_buffer(Buffer *buffer, char *string) {
    if (!buffer->value) {
        buffer->value = malloc(1024);
        buffer->buffer_capacity = 1024;
        reset_buffer(buffer);
    }

    int string_size = strlen(string);

    while (buffer->buffer_size + string_size + 1 >= buffer->buffer_capacity) {
        buffer->buffer_capacity *= 2;
        buffer->value = realloc(buffer->value, buffer->buffer_capacity);
    }

    while (*string) {
        buffer->value[buffer->buffer_size++] = *string++;
    }

    buffer->value[buffer->buffer_size] = '\0';
}

/**
 * Frees the buffer.
 */
void free_buffer(Buffer *buffer) {
    if (buffer->value) {
        free(buffer->value);
    }
}

/**
 * Struct that represents an output writer.
 */
struct OutputWriter {
    void *target;
    void (*write_token)(OutputWriter *, const char *, va_list);
    void (*write_token_end)(OutputWriter *, const char *, va_list);

    Buffer buffer;
};

/**
 * Writes a token to the file without ending line.
 */
static void write_token_semicolon_file_output_writer(OutputWriter *output_writer, const char *format, va_list args) {
    vfprintf(output_writer->target, format, args);
    fprintf(output_writer->target, ";");
}

/**
 * Writes a token to the file and ends line.
 */
static void write_token_end_semicolon_file_output_writer(OutputWriter *output_writer, const char *format, va_list args) {
    vfprintf(output_writer->target, format, args);
    fprintf(output_writer->target, "\n");
}

/**
 * Writes a token to the buffer without ending line.
 */
static void write_token_array_of_semicolon_strings_output_writer(OutputWriter *output_writer, const char *format, va_list args) {
    char string[1024]; // Truncate the string if it is bigger than 1024 characters by design
    vsnprintf(string, 1024, format, args);

    append_to_buffer(&output_writer->buffer, string);
    append_to_buffer(&output_writer->buffer, ";");
}

/**
 * Writes a token to the buffer, ends line, adds the buffer to the array and resets the buffer.
 */
static void write_token_end_array_of_semicolon_strings_output_writer(OutputWriter *output_writer, const char *format, va_list args) {
    char string[1024]; // Truncate the string if it is bigger than 1024 characters by design
    vsnprintf(string, 1024, format, args);

    append_to_buffer(&output_writer->buffer, string);
    append_to_buffer(&output_writer->buffer, "\n");

    // Add the string to the array
    GPtrArray *array = output_writer->target;
    g_ptr_array_add(array, g_strdup(output_writer->buffer.value));

    // Reset value
    reset_buffer(&output_writer->buffer);
}

OutputWriter *create_semicolon_file_output_writer(FILE *file) {
    OutputWriter *output_writer = malloc(sizeof(OutputWriter));
    output_writer->target = file;
    output_writer->write_token = write_token_semicolon_file_output_writer;
    output_writer->write_token_end = write_token_end_semicolon_file_output_writer;
    output_writer->buffer = init_buffer();
    return output_writer;
}

OutputWriter *create_array_of_semicolon_strings_output_writer(GPtrArray *array) {
    OutputWriter *output_writer = malloc(sizeof(OutputWriter));
    output_writer->target = array;
    output_writer->write_token = write_token_array_of_semicolon_strings_output_writer;
    output_writer->write_token_end = write_token_end_array_of_semicolon_strings_output_writer;
    output_writer->buffer = init_buffer();
    return output_writer;
}

OutputWriter *create_null_output_writer(void) {
    OutputWriter *output_writer = malloc(sizeof(OutputWriter));
    output_writer->target = NULL;
    output_writer->write_token = NULL;
    output_writer->write_token_end = NULL;
    output_writer->buffer = init_buffer();
    return output_writer;
}

void writer_write_output_token(OutputWriter *output_writer, const char *format, ...) {
    if (output_writer->write_token) {
        va_list args;
        va_start(args, format);
        output_writer->write_token(output_writer, format, args);
        va_end(args);
    }
}

void writer_write_output_token_end(OutputWriter *output_writer, const char *format, ...) {
    if (output_writer->write_token_end) {
        va_list args;
        va_start(args, format);
        output_writer->write_token_end(output_writer, format, args);
        va_end(args);
    }
}

void *output_writer_get_target(OutputWriter *output_writer) {
    return output_writer->target;
}

void close_output_writer(OutputWriter *output_writer) {
    free_buffer(&output_writer->buffer);
    free(output_writer);
}
