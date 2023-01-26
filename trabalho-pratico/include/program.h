#pragma once
#ifndef LI3_PROGRAM_H
#define LI3_PROGRAM_H

#include <glib.h>
#include <stdio.h>
#include <math.h>

#include "program_flags.h"

typedef struct Program Program;

typedef enum ProgramState {
    PROGRAM_STATE_RUNNING,
    PROGRAM_STATE_EXITING,
} ProgramState;

void program_set_state(Program *program, ProgramState state);

void program_set_should_exit(Program *program, gboolean should_exit);

gboolean program_should_exit(Program *program);

Program *create_program(ProgramFlags *flags);

void free_program(Program *program);

int start_program(Program *program, GPtrArray *program_args);

gboolean program_load_dataset(Program *program, char *dataset_folder_path);

gboolean program_run_queries_from_file(Program *program, char *input_file_path);

#endif //LI3_PROGRAM_H
