#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"

static struct argp_option options[] = {
    {"dump-elf-header",       'e', 0, 0,  "Dumps the elf header to stdout" },
    {"dump-section-header",   's', 0, 0,  "Dumps the section header to stdout" },
    {"dump-program-header",   'p', 0, 0,  "Dumps the program header to stdout" },
    {"dump-symbol-table",     't', 0, 0,  "Dumps the symbol table to stdout" },
    { 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct args *arguments = state->input;

    switch (key) {
        case 'e':
            arguments->dump_header = 1;
            break;
        case 's':
            arguments->dump_section_header = 1;
            break;
        case 'p':
            arguments->dump_program_header = 1;
            break;
        case 't':
            arguments->dump_symtab = 1;
            break;
        case ARGP_KEY_ARG:
            char message_buffer[1000];
            char *cur_arg = arg;

            snprintf(message_buffer, sizeof(message_buffer), "ERROR: filepath '%s' doesn't exist!", cur_arg);
            // assume is the filepath if not a flag
            if (!file_exists(cur_arg)) { fatal_error(message_buffer); }
            if (!file_readable(cur_arg)) { fatal_error("ERROR: unable to read file!"); }

            int len = strlen(cur_arg);
            char *filepath = malloc(sizeof(char) * len);
            strcpy(filepath, cur_arg);

            arguments->path.size = len;
            arguments->path.filepath = filepath;
            arguments->path.set = true;
            return 0;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, "ARG1 ARG2", "A program to demonstrate argp" };
Args parse_args(int argc, char *argv[]) {
    Args ret = {
        .dump_header = false,
        .dump_section_header = false,
        .dump_program_header = false,
        .dump_symtab = false,
        .path = {
            .set = false,
        },
    };
    argp_parse(&argp, argc, argv, 0, 0, &ret);
    return ret;

    /*
    for (int i = 1; i < argc; i++) {
        char *cur_arg = argv[i];

        if (strcmp(cur_arg, DUMP_HEADER_ARG) == 0 || strcmp(cur_arg, DUMP_HEADER_ARG_LONG) == 0) {
            ret.dump_header = true;
        } else if (strcmp(cur_arg, DUMP_SECTION_HEADER_ARG) == 0 || strcmp(cur_arg, DUMP_SECTION_HEADER_ARG_LONG) == 0) {
            ret.dump_section_header = true;
        } else if (strcmp(cur_arg, DUMP_PROGRAM_HEADER_ARG) == 0 || strcmp(cur_arg, DUMP_PROGRAM_HEADER_ARG_LONG) == 0) {
            ret.dump_program_header = true;
        } else if (strcmp(cur_arg, DUMP_SYMBOL_TABLE_ARG) == 0 || strcmp(cur_arg, DUMP_SYMBOL_TABLE_ARG_LONG) == 0) {
            ret.dump_symtab = true;
        } else {
            char message_buffer[1000];

            snprintf(message_buffer, sizeof(message_buffer), "ERROR: filepath '%s' doesn't exist!", cur_arg);
            // assume is the filepath if not a flag
            if (!file_exists(cur_arg)) { fatal_error(message_buffer); }
            if (!file_readable(cur_arg)) { fatal_error("ERROR: unable to read file!"); }

            int len = strlen(cur_arg);
            char *filepath = malloc(sizeof(char) * len);
            strcpy(filepath, cur_arg);

            ret.path.size = len;
            ret.path.filepath = filepath;
            ret.path.set = true;
        }
    }

    if (!ret.path.set) { fatal_error("ERROR: user did not supply object filepath"); }

    return ret;
    */
}
