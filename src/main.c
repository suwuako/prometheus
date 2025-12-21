#include <stdio.h>
#include <stdlib.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"
#include "../headers/dump_elf_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/dump_program_header.h"

int main(int argc, char *argv[]) {
    printf("== hello, dump == \n\n");
    Args args = parse_args(argc, argv);

    print_args(args);

    Elf_header elf_header = grab_elf_header(args);
    Section_header *section_headers = grab_all_section_headers(elf_header, args);
    bool proghead_exists = program_header_exists(elf_header);
    Program_header *program_headers = NULL;
    if (proghead_exists) 
        program_headers = grab_program_headers(elf_header, args);


    if (!proghead_exists && args.dump_program_header) fatal_error("-p doesn't work with object files without program headers!");
    if (args.dump_header) dump_header(elf_header);
    if (args.dump_section_header) dump_section_headers(section_headers, elf_header, args);
    if (args.dump_program_header) dump_program_headers(program_headers, elf_header, args);


    if (program_headers != NULL) free(program_headers);
    free(section_headers);

    return 0;
}
