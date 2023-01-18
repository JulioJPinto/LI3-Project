#pragma once
#ifndef LI3_OUTPUT_WRITER_H
#define LI3_OUTPUT_WRITER_H

typedef struct OutputWriter OutputWriter;

OutputWriter *create_file_output_writer(FILE* file);

OutputWriter *create_array_of_strings_output_writer(void);

void write_output_line(OutputWriter *buffer, const char *format, ...);

void close_output_writer(OutputWriter *buffer);

#endif //LI3_OUTPUT_WRITER_H
