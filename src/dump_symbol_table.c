#include <stdio.h>
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

Elf64_Sym *grab_symbol_table(Elf_header elf_header, Section_header *section_header, Args args) {
    FILE *fd = fopen(args.path.filepath, "r");
    Section_header sh_symtab = get_symtab_entry(section_header, elf_header);
    int entries = sh_symtab.sh_size / sh_symtab.sh_entsize;
    navigate_fd_to_symbol_table(fd, sh_symtab);

    for (int i = 0; i < entries; i++) {
        Elf64_Sym symtab_entry = read_symbol_entry(fd, sh_symtab, elf_header);
    }
}
