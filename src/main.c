#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "elf_header.h"
#include "custom_objdump.h"

int main(int argc, char *argv[]) {
    // todo fixup clean

    char *name = parse_filename(argc, argv);
    custom_objdump(name);
}
