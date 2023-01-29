#pragma once
#ifndef LI3_OUTPUT_WRITER_H
#define LI3_OUTPUT_WRITER_H

/**
 * Abstraction of a buffer that can be written to.
 * This buffer can be a file, an array of strings or a null buffer.
 * Used to write the output of the queries.
 */
typedef struct OutputWriter OutputWriter;

/**
 * Creates a new OutputWriter that writes to the given file.
 */
OutputWriter *create_file_output_writer(FILE *file);

/**
 * Creates a new OutputWriter that writes to an array of strings.
 * The array of strings is allocated in the heap memory and can be retrieved with the `get_buffer` function.
 */
OutputWriter *create_array_of_strings_output_writer(void);

/**
 * Creates a new OutputWriter that writes to a null buffer (does not write anything).
 */
OutputWriter *create_null_output_writer(void);

/**
 * Writes a line to the output writer.
 * The line is formatted with the given format and the following arguments.
 */
void write_output_line(OutputWriter *output_writer, const char *format, ...);

/**
 * Returns the buffer associated with the output writer.
 * The buffer is an array of strings if the output writer is an array of strings output writer.
 * The buffer is a file if the output writer is a file output writer.
 * The buffer is NULL if the output writer is a null output writer.
 */
void *get_buffer(OutputWriter *output_writer);

/**
 * Closes the output writer.
 * This function must be called when the output writer is no longer needed.
 * This function frees the memory allocated for the output writer and the buffer if it is an array of strings output writer.
 * This function does not close the file if it is a file output writer.
 */
void close_output_writer(OutputWriter *output_writer);

#endif //LI3_OUTPUT_WRITER_H
