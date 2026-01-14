#ifndef OBJ_METADATA_H
#define OBJ_METADATA_H

#include "dump_elf_header.h"
#include "dump_symbol_table.h"
#include "dump_program_header.h"
#include "dump_section_header.h"

typedef struct objdata {
    char *path;
    Elf_header eheader;
    Section_header *sheaders;
    Program_header *pheaders;
    Hashmap **symtab;
    struct objdata *next;
} Objdata;


Objdata *get_objdata(Args *args);
Objdata *new_objdata(Elf_header e, Section_header *s, Program_header *p, Hashmap **sym, char *fn);
void dump_objdata(Objdata *obj, Args args);

#endif
