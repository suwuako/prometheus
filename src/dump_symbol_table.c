#include <stdio.h>
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

char *get_symbol_name(Section_header *strtab, int offset) {

}

Hashmap **grab_symbol_table(Elf_header elf_header, Section_header *section_header, Args args) {
    Hashmap **ret = hashmap_new();
    FILE *fd = fopen(args.path.filepath, "r");
    Section_header sh_symtab = get_section_entry(section_header, elf_header, 0x2);
    int entries = sh_symtab.sh_size / sh_symtab.sh_entsize;
    navigate_fd_to_symbol_table(fd, sh_symtab);

    for (int i = 0; i < entries; i++) {
        Elf64_Sym symtab_entry = read_symbol_entry(fd, sh_symtab, elf_header);
    }

    return ret;
}
