#pragma once
#ifndef LI3_OUTPUT_WRITER_H
#define LI3_OUTPUT_WRITER_H

typedef struct OutputWriter OutputWriter;

OutputWriter *create_file_output_writer(FILE *file);

OutputWriter *create_array_of_strings_output_writer(void);

OutputWriter *create_null_output_writer(void);

void write_output_line(OutputWriter *output_writer, const char *format, ...);

void *get_buffer(OutputWriter *output_writer);

void close_output_writer(OutputWriter *output_writer);

#endif //LI3_OUTPUT_WRITER_H
