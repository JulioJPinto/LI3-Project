#pragma once
#ifndef LI3_PROGRAM_FLAGS_H
#define LI3_PROGRAM_FLAGS_H

#include <glib.h>

/**
 * Struct that holds the program flags.
 * The flags are used in the program.
 * The current implementation is just a wrapper around an hash table.
 */
typedef struct ProgramFlags ProgramFlags;

/**
 * Parses the program flags (words that start with `--`) from the given program arguments and removes them from the array.
 * Returns a ProgramFlags struct that holds the flags.
 */
ProgramFlags *steal_program_flags(GPtrArray *program_args);

/**
 * Returns the value of the given flag.
 * If the flag is not present, returns the default value.
 */
char *get_program_flag_value(ProgramFlags *program_flags, char *flag, char *default_value);

/**
 * Frees the memory allocated for the program flags.
 */
void free_program_flags(ProgramFlags *program_flags);

#endif //LI3_PROGRAM_FLAGS_H
