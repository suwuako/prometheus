#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "../headers/dump_section_header.h"
#include "../headers/dump_program_header.h"
#include "../headers/dump_elf_header.h"

// e_phnum Contains the number of entries in the program header table. 
// if e_phnum is 0, program header doesn't exist
bool program_header_exists(Elf_header header) {
    if (header.e_phnum == 0) return false;
    return true;
}

void navigate_fd_to_program_header(Elf_header header, FILE *fd) {
    uint64_t entry_point = header.e_phoff;

    lseek(fileno(fd), entry_point, SEEK_SET);
}

void navigate_fd_to_program_index(Elf_header header, FILE *fd, int i) {
    lseek(fileno(fd), i * header.e_phentsize, SEEK_CUR);
}

Program_header *grab_program_headers(Elf_header header, Args args) {
    Program_header *ret = calloc(sizeof(Program_header), header.e_phnum);

    for (int i = 0; i < header.e_phnum; i++) {
        Program_header prog = grab_program_header(header, args, i);
        ret[i] = prog;
    }

    return ret;
}

Program_header grab_program_header(Elf_header header, Args args, int index) {
    Program_header ret = {};
    uint8_t BYTE_FORMAT = header.ei_class;
    FILE *fd = fopen(args.path.filepath, "r");
    navigate_fd_to_program_header(header, fd);
    navigate_fd_to_program_index(header, fd, index);

    ret.p_type = read_nbytes_better(header, fd, 4, false);
    if (BYTE_FORMAT == SIXTY_FOUR_BIT) ret.p_flags = read_nbytes_better(header, fd, 4, false);
    ret.p_offset = read_nbytes_better(header, fd, 0, true);
    ret.p_vaddr = read_nbytes_better(header, fd, 0, true);
    ret.p_paddr = read_nbytes_better(header, fd, 0, true);
    ret.p_filesz = read_nbytes_better(header, fd, 0, true);
    ret.p_memsz = read_nbytes_better(header, fd, 0, true);
    if (BYTE_FORMAT == THIRTY_TWO_BIT) ret.p_flags = read_nbytes_better(header, fd, 4, false);
    ret.p_align = read_nbytes_better(header, fd, 0, true);

    return ret;
}

const char* get_p_type_name(uint32_t p_type) {
    switch (p_type) {
        case PT_NULL:    return "NULL (Unused)";
        case PT_LOAD:    return "LOAD (Loadable segment)";
        case PT_DYNAMIC: return "DYNAMIC (Dynamic linking info)";
        case PT_INTERP:  return "INTERP (Interpreter info)";
        case PT_NOTE:    return "NOTE (Auxiliary info)";
        case PT_SHLIB:   return "SHLIB (Reserved)";
        case PT_PHDR:    return "PHDR (Program header table)";
        case PT_TLS:     return "TLS (Thread-Local Storage)";
        
        /* Range handling for OS and Processor specific types */
        default:
            if (p_type >= PT_LOOS && p_type <= PT_HIOS) {
                return "OS Specific";
            } else if (p_type >= PT_LOPROC && p_type <= PT_HIPROC) {
                return "Processor Specific";
            } else {
                return "UNKNOWN";
            }
    }
}

char* get_p_flags_name(uint64_t p_flags) {
    char *ret = malloc(sizeof(char) * 4);
    if ((p_flags & 0x4)) {
        ret[0] = 'r';
    } else {
        ret[0] = '-';
    }

    if ((p_flags & 0x2)) {
        ret[1] = 'w';
    } else {
        ret[1] = '-';
    }

    if ((p_flags & 0x1)) {
        ret[2] = 'x';
    } else {
        ret[2] = '-';
    }
    ret[3] = '\0';
    return ret;
}

void print_program_header(Program_header ph) {
    printf("%s\n", get_p_type_name(ph.p_type));
    printf("File Offset:    0x%16lx\n", ph.p_offset);
    printf("Virtual Addr:   0x%16lx\n", ph.p_vaddr);
    printf("Physical Addr:  0x%16lx\n", ph.p_paddr);
    printf("File size:      0x%16lx\n", ph.p_filesz);
    printf("Memory size:    0x%16lx\n", ph.p_memsz);
    char *flags = get_p_flags_name(ph.p_flags);
    printf("Flags:          %s\n", flags);
    free(flags);
    printf("Alignment:      %ld\n", ph.p_align);
    printf("------------------------------------\n");
}

void dump_program_headers(Program_header *ph, Elf_header eh, Args args) {
    printf("\n== program header dump ==\n\n");
    printf("Index Type Flags Offset Vaddr Paddr Filesz Memsz Align\n");

    for (int i = 0; i < eh.e_phnum; i++) {
        print_program_header(ph[i]);
    }
}
