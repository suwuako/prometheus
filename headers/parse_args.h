#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <stdbool.h>
#include <argp.h>


#define DUMP_HEADER_ARG "-h"
#define DUMP_HEADER_ARG_LONG "--dump-header"
#define DUMP_SECTION_HEADER_ARG "-s"
#define DUMP_SECTION_HEADER_ARG_LONG "--dump-section-header"
#define DUMP_PROGRAM_HEADER_ARG "-p"
#define DUMP_PROGRAM_HEADER_ARG_LONG "--dump-program-header"
#define DUMP_SYMBOL_TABLE_ARG "-t"
#define DUMP_SYMBOL_TABLE_ARG_LONG "--dump-symbol-table"

typedef struct filepath {
    char *filepath;
    int size;
    bool set;             // is set when valid filepath is set
} Filepath;


typedef struct args {
    Filepath path;
    bool dump_header;
    bool dump_section_header;
    bool dump_program_header;
    bool dump_symtab;
} Args;


Args parse_args(int argc, char *argv[]);

#endif
