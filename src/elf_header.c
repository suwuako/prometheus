#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "elf_header.h"
#include "types.h"

// reads the first 24 bytes until entry sizes diverse because of 32bit/64bit
#define INITIAL_HEADER_SIZE 24

void poll_header(FILE *obj) {
    // read from 0x00 -> 0x18 before diverging off 32/64bit entries
    // reads 24 bytes into header first
    struct elf_header_content header;
    bool is_64bit;
    int chars_read = 0;
    int c;

    // magic bytes
    for (int i = 0; i < ELF_MAGIC_SIZE; i++) {
        header.EI_MAG[i] = fgetc(obj);
    }
    if (check_magic(header)) { exit(1); }

    header.EI_CLASS = fgetc(obj);
    header.EI_DATA = fgetc(obj);
    header.EI_VERSION = fgetc(obj);
    header.EI_OSABI = fgetc(obj);
    header.EI_ABIVERSION = fgetc(obj);

    // padding for 7 bytes
    for (int i = 0; i < 7; i++) {
        header.EI_PAD[i] = fgetc(obj);
    }

    header.type[0] = fgetc(obj);
    header.type[1] = fgetc(obj);
    header.machine[0] = fgetc(obj);
    header.machine[1] = fgetc(obj);

    for (int i = 0; i < 4; i++) {
        header.version[i] = fgetc(obj);
    }
}

bool check_magic(struct elf_header_content f) {
    const int ELF_MAGIC[ELF_MAGIC_SIZE] = {0x7f, 'E', 'L', 'F'};
    for (int i = 0; i < ELF_MAGIC_SIZE; i++) {
        if (f.EI_MAG[i] != ELF_MAGIC[i]) { return false; }
    }
    return true;
}
