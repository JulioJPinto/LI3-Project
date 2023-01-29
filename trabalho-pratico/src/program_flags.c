#include "program_flags.h"

#include <glib.h>
#include "logger.h"
#include "string_util.h"

/**
 * A struct that contains all the flags passed to the program
 */
struct ProgramFlags {
    GHashTable *flags;
};

/**
 * Parses a flag in the form of "flag=value" into a flag and a value
 */
gboolean parse_flag(const char *complete_flag, char **flag_result, char **value_result) {
    char **split = g_strsplit(complete_flag, "=", 0);
    guint length = g_strv_length(split);
    if (length != 2) {
        log_panic("Invalid flag '%s'\n", complete_flag);
        return FALSE;
    }
    *flag_result = g_strdup(split[0]);
    str_to_lower(*flag_result);
    *value_result = g_strdup(split[1]);
    str_to_lower(*value_result);
    g_strfreev(split);
    return TRUE;
}

ProgramFlags *steal_program_flags(GPtrArray *program_args) {
    ProgramFlags *program_flags = g_malloc(sizeof(ProgramFlags));
    program_flags->flags = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    for (int i = (int) program_args->len - 1; i >= 0; i--) { // loop backwards to be able to remove element from array
        char *arg = g_ptr_array_index(program_args, i);
        if (g_str_has_prefix(arg, "--")) {
            char *flag, *value;
            if (parse_flag(arg + 2, &flag, &value)) {
                g_hash_table_insert(program_flags->flags, flag, value);
                g_ptr_array_remove_index(program_args, i);
            }
        }
    }

    return program_flags;
}

char *get_program_flag_value(ProgramFlags *program_flags, char *flag, char *default_value) {
    char *value = g_hash_table_lookup(program_flags->flags, flag);
    if (value == NULL) {
        return default_value;
    }
    return value;
}

void free_program_flags(ProgramFlags *program_flags) {
    g_hash_table_destroy(program_flags->flags);
    g_free(program_flags);
}
