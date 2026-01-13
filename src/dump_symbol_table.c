#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/dump_symbol_table.h"

void navigate_fd_to_symbol_table(FILE *fd, Section_header sh) {
    lseek(fileno(fd), sh.sh_offset, SEEK_SET);
}

// gets the section header entry which points to symtab
Section_header get_symtab_entry(Section_header *sh, Elf_header elf_header) {
    for (int i = 0; i < elf_header.e_shnum; i++) {
        if (sh[i].sh_type == 0x2) {
            return sh[i];
        }
    }

    fatal_error("could not find symtab in section headers?");
}

Elf64_Sym read_symbol_entry(FILE *fd, Section_header sh_symtab, Elf_header header) {
    int entry_size = sh_symtab.sh_entsize;
    
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

char *get_symbol_name(Elf_header ehead, Section_header *shead, int offset, Args args) {
    Section_header strtab = get_section_entry(shead, ehead, 0x3);
    int file_offset = strtab.sh_offset;
    FILE *fd = fopen(args.path.filepath, "r");
    lseek(fileno(fd), file_offset + offset, SEEK_SET);

    int strlen = 0;
    char c;

    while ((c = fgetc(fd)) != '\0') {
        strlen++;
    }
    lseek(fileno(fd), file_offset + offset, SEEK_SET);
    char *ret = calloc(strlen, sizeof(char));
    for (int i = 0; i < strlen; i++) {
        ret[i] = fgetc(fd);
    }

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

        char *name = get_symbol_name(elf_header, section_header, symtab_entry.st_name, args);
        hashmap_insert(name, entry, ret);
    }

    return ret;
}
