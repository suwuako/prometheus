#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/dump_symbol_table.h"

void navigate_fd_to_symbol_table(FILE *fd, Section_header sh) {
    lseek(fileno(fd), sh.sh_offset, SEEK_SET);
}


Elf64_Sym read_symbol_entry(FILE *fd, Section_header sh_symtab, Elf_header header) {
    Elf64_Sym ret = {
        .st_name = read_nbytes_better(header, fd, 4, 0),
        .st_info = read_nbytes_better(header, fd, 1, 0),
        .st_other = read_nbytes_better(header, fd, 1, 0),
        .st_shndx = read_nbytes_better(header, fd, 2, 0),
        .st_value = read_nbytes_better(header, fd, 8, 0),
        .st_size = read_nbytes_better(header, fd, 8, 0),
    };

    return ret;
}

char *get_symbol_name(Elf_header ehead, Section_header shead, int offset, Args args) {
    Section_header strtab = shead;
    char buf[10000] = {};
    int i = 0;
    int len;
    char c;
    int file_offset = strtab.sh_offset;
    FILE *fd = fopen(args.path.filepath, "r");

    fseek(fd, file_offset + offset, SEEK_SET);
    while ((c = fgetc(fd)) != '\0' && i < 10000) {
        buf[i] = c;
        i++;
    }

    len = strlen(buf);
    printf("%s, %d\n", buf, len);

    char *ret = calloc(len + 1, sizeof(char));
    for (int i = 0; i < len; i++) {
        ret[i] = buf[i];
    }
    ret[len] = '\0';

    return ret;
}

Hashmap **grab_symbol_table(Elf_header elf_header, Section_header *section_header, Args args) {
    Hashmap **ret = hashmap_new();
    FILE *fd = fopen(args.path.filepath, "r");
    Section_header sh_symtab = get_section_entry(section_header, elf_header, 0x2);
    int entries = sh_symtab.sh_size / sh_symtab.sh_entsize;
    navigate_fd_to_symbol_table(fd, sh_symtab);

    for (int i = 0; i < entries; i++) {
        Elf64_Sym *entry = malloc(sizeof(Elf64_Sym));
        Elf64_Sym symtab_entry = read_symbol_entry(fd, sh_symtab, elf_header);
        memcpy(entry, &symtab_entry, sizeof(Elf64_Sym));

        char *name = get_symbol_name(elf_header, section_header[sh_symtab.sh_link], symtab_entry.st_name, args);
        hashmap_insert(name, entry, ret);
    }
    printf("done!\n");

    return ret;
}
