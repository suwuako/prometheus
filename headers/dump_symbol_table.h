#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <elf.h>

#include "hashmap.h"
#include "dump_elf_header.h"
#include "dump_section_header.h"


typedef struct symtab_entry {
    Elf64_Sym entry;
    char *name;
} Symtab_entry;

void navigate_fd_to_symbol_table(FILE *fd, Section_header sh);
Hashmap **grab_symbol_table(Elf_header elf_header, Section_header *section_header, Args args);
char *get_symbol_name(Elf_header ehead, Section_header shead, int offset, Args args);

#endif
