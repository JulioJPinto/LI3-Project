#pragma once
#ifndef LI3_OUTPUT_WRITER_H
#define LI3_OUTPUT_WRITER_H

/**
 * Struct that represents an output writer.
 *
 * This module provides a way to write output to different targets.
 * The output writer can be used to write output to a semicolon separated file, to an array of strings, or to a null target.
 */
typedef struct OutputWriter OutputWriter;

/**
 * Creates an output writer that writes to the file.
 *
 * The output writer will write to the file directly with no overhead,
 * separating tokens with semicolons and ending lines with newlines.
 *
 * The output writer will not close the file when close_output_writer is called.
 */
OutputWriter *create_semicolon_file_output_writer(FILE *file);

/**
 * Creates an output writer that writes to an array of strings separated by semicolons.
 *
 * The output writer will write buffer, separating tokens with semicolons and ending lines with newlines.
 * Only when the end function is called, the string is added to the array.
 *
 * The output writer will not free the array when close_output_writer is called.
 */
OutputWriter *create_array_of_semicolon_strings_output_writer(GPtrArray *array);

/**
 * Creates an output writer that writes to a null target.
 * The output writer will simply ignore write calls.
 * Used when testing query performance.
 */
OutputWriter *create_null_output_writer(void);

/**
 * Writes a token to the output writer depending on the implementation chosen when creating the output writer.
 */
void writer_write_output_token(OutputWriter *output_writer, const char *format, ...);

/**
 * Writes a token to the output writer and terminates the line
 * depending on the implementation defined when creating the output writer.
 */
void writer_write_output_token_end(OutputWriter *output_writer, const char *format, ...);

/**
 * Frees the allocated memory of the output writer.
 */
void close_output_writer(OutputWriter *output_writer);

#endif //LI3_OUTPUT_WRITER_H
