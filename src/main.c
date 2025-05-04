#include <stdio.h>
#include <stdlib.h>

#include "extra.h"
#include "consts.h"
#include "elf.h"

int main(int argc, char *argv[]) {
    // todo fixup clean
    FILE *f = fopen(argv[1], "r");
    if (!check_magic(f)) {
        fprintf(stderr, "File is not an ELF!\n");
        exit(1);
    }
}
