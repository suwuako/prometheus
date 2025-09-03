#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elf_header.h"
#include "custom_objdump.h"

char *parse_filename(int argc, char *argv[]) {
    // assume is arg1
    int len = strlen(argv[1]);
    char *name = malloc(sizeof(char) * len);
    strcpy(name, argv[1]);
    return name;
}

void insert_n_times(FILE *f, struct sizetypes *entry, int n) {
    for (int i = 0; i < n; i++) {
        shift_and_insert(f, entry);
    }
}

void shift_and_insert(FILE *f, struct sizetypes *entry) {
    entry->data <<= 8;
    entry->data |= fgetc(f);
}

struct elf_header custom_objdump(char *name) {
    FILE *f = fopen(name, "r");
    struct elf_header header;

    header.EI_MAG[0] = fgetc(f);
    header.EI_MAG[1] = fgetc(f);
    header.EI_MAG[2] = fgetc(f);
    header.EI_MAG[3] = fgetc(f);

    header.EI_CLASS = fgetc(f);
    header.EI_DATA = fgetc(f);
    header.EI_VERSION = fgetc(f);
    header.EI_OSABI = fgetc(f);
    header.EI_ABIVERSION = fgetc(f);

    insert_n_times(f, &header.e_type, 2);
    header.e_type.type = UNDEFINED;
    insert_n_times(f, &header.e_machine, 2);
    header.e_machine.type = UNDEFINED;
    insert_n_times(f, &header.e_version, 4);
    header.e_version.type = UNDEFINED;

    if (header.EI_CLASS == FOURBYTES) {
        insert_n_times(f, &header.e_entry, 4);
        insert_n_times(f, &header.e_phoff, 4);
        insert_n_times(f, &header.e_shoff, 4);
    } else if (header.EI_CLASS == EIGHTBYTES) {
        insert_n_times(f, &header.e_entry, 8);
        insert_n_times(f, &header.e_phoff, 8);
        insert_n_times(f, &header.e_shoff, 8);
    }

    insert_n_times(f, &header.e_shoff, 4);

    return header;
}
