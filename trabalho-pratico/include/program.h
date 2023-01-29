#pragma once
#ifndef LI3_PROGRAM_H
#define LI3_PROGRAM_H

#include <glib.h>
#include <stdio.h>
#include <math.h>

#include "program_flags.h"

/**
 * Struct that represents a program.
 * The program is responsible for running the application.
 */
typedef struct Program Program;

/**
 * Enum that represents the state of the program.
 */
typedef enum ProgramState {
    PROGRAM_STATE_RUNNING,
    PROGRAM_STATE_EXITING,
} ProgramState;

/**
 * Sets the state of the program.
 * If the state is PROGRAM_STATE_EXITING, the program will exit its loop.
 * The application will only closed if `program_should_exit` returns true otherwise it will restart.
 */
void program_set_state(Program *program, ProgramState state);

/**
 * Sets if the program should exit when it is in the PROGRAM_STATE_EXITING state.
 * This function is used to restart the program when `should_exit` is false.
 */
void program_set_should_exit(Program *program, gboolean should_exit);

/**
 * Returns true if the program should exit or false if it should restart.
 */
gboolean program_should_exit(Program *program);

/**
 * Creates a new Program struct.
 * The program also holds the catalog of the application.
 */
Program *create_program(ProgramFlags *flags);

/**
 * Frees the memory allocated for the program (including the created catalog).
 */
void free_program(Program *program);

/**
 * Starts the program with the given flags.
 * 
 * Available flags:
 * - `--lazy-loading=true` (default): Only index/sort catalog when needed (when a query is run).
 * - `--lazy-loading=false`: Index/sort everything after loading the dataset.
 */
int start_program(Program *program, GPtrArray *program_args);

/**
 * Loads the dataset from the given folder path.
 * Returns true if the dataset was loaded successfully.
 * This function registers all the users, drivers, rides and other data needed in the catalog.
 */
gboolean program_load_dataset(Program *program, char *dataset_folder_path);

/**
 * Runs the queries from the given input file.
 * Returns true if the file was read successfully.
 */
gboolean program_run_queries_from_file(Program *program, char *input_file_path);

#endif //LI3_PROGRAM_H
