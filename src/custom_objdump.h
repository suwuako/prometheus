#ifndef CUSTOM_OBJDUMP_H
#define CUSTOM_OBJDUMP_H

#include <stdio.h>
#include "elf_header.h"

char *parse_filename(int argc, char *argv[]);
void insert_n_times(FILE *f, struct sizetypes *entry, int n);
void shift_and_insert(FILE *f, struct sizetypes *entry);
struct elf_header custom_objdump(char *name);


#endif
