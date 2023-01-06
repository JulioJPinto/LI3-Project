#pragma once
#ifndef LI3_PROGRAM_H
#define LI3_PROGRAM_H

#include "catalog.h"

typedef struct Program Program;

Program *create_program(void);

void free_program(Program *program);

void run_program(Program *program, char **args, int arg_size);

gboolean program_load_dataset(Program *program, char *dataset_folder_path);

void program_run_query(Program *program, char *query);

gboolean program_run_queries_from_file(Program *program, char *input_file_path);

#endif //LI3_PROGRAM_H
