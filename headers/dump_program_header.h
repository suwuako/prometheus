#ifndef DUMP_PROGRAM_HEADER_H
#define DUMP_PROGRAM_HEADER_H

#include "dump_elf_header.h"
#include "parse_args.h"

#define PT_NULL         0x00000000  /* Program header table entry unused */
#define PT_LOAD         0x00000001  /* Loadable program segment */
#define PT_DYNAMIC      0x00000002  /* Dynamic linking information */
#define PT_INTERP       0x00000003  /* Program interpreter path name */
#define PT_NOTE         0x00000004  /* Auxiliary information */
#define PT_SHLIB        0x00000005  /* Reserved */
#define PT_PHDR         0x00000006  /* Entry for header table itself */
#define PT_TLS          0x00000007  /* Thread lcoal storage template */
#define PT_LOOS         0x60000000  /* Start of OS-specific */
#define PT_HIOS         0x6FFFFFFF  /* End of OS-specific */
#define PT_LOPROC       0x70000000  /* Start of processor-specific */
#define PT_HIPROC       0x7FFFFFFF



typedef struct program_header {
    uint64_t p_type;
    uint64_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Program_header;

bool program_header_exists(Elf_header header);
Program_header *grab_program_headers(Elf_header header, Args args);
Program_header grab_program_header(Elf_header header, Args args, int index);
void navigate_fd_to_program_header(Elf_header header, FILE *fd);
void navigate_fd_to_program_index(Elf_header header, FILE *fd, int i);
void dump_program_headers(Program_header *ph, Elf_header eh, Args args);

#endif
