#pragma once
#ifndef LI3_PROGRAM_H
#define LI3_PROGRAM_H

#include <glib.h>
#include <stdio.h>

#include "program_flags.h"

typedef struct Program Program;

gboolean program_should_exit(Program *program);

Program *create_program(ProgramFlags *flags);

void free_program(Program *program);

int start_program(Program *program, GPtrArray *program_args);

gboolean program_load_dataset(Program *program, char *dataset_folder_path);

void program_run_query(Program *program, char *query);

gboolean program_run_queries_from_file(Program *program, char *input_file_path);

#endif //LI3_PROGRAM_H
