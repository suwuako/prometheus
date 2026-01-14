#ifndef DUMP_PROGRAM_HEADER_H
#define DUMP_PROGRAM_HEADER_H

#include <elf.h>


#include "dump_elf_header.h"
#include "parse_args.h"


typedef struct program_header {
    uint64_t p_type;
    uint64_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Program_header;

bool program_header_exists(Elf_header header);
Program_header *grab_program_headers(Elf_header header, Args args, char *fn);
void navigate_fd_to_program_header(Elf_header header, FILE *fd);
void navigate_fd_to_program_index(Elf_header header, FILE *fd, int i);
void dump_program_headers(Program_header *ph, Elf_header eh, Args args);
Program_header grab_program_header(Elf_header header, Args args, int index, char *fn);

#endif
