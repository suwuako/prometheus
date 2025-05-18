#ifndef ELF_H
#define ELF_H

#include <stdbool.h>
#include <stdio.h>

#include "types.h"


void poll_header(FILE *obj);
bool check_magic(struct elf_header_content f);


#endif
