#ifndef ELF_H
#define ELF_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


/*
void poll_header(FILE *obj);
bool check_magic(struct elf_header_content f);
*/

#define UNDEFINED 0
#define FOURBYTES 1
#define EIGHTBYTES 2
#define TWOBYTES 3;


struct sizetypes {
    int type;
    uint64_t data;
};

struct elf_header {
    char                             EI_MAG[4];      // magic bytes
    char                             EI_CLASS;      // 32/64 bit
    char                             EI_DATA;       // endianess 
    char                             EI_VERSION;    // elf version
    char                             EI_OSABI;      // os abi
    char                             EI_ABIVERSION;  // additional abi version
                                          // padding bytes are ignored (7 bytes)
    struct sizetypes                 e_type;
    struct sizetypes                 e_machine;
    struct sizetypes                 e_version;
    struct sizetypes                 e_entry;
    struct sizetypes                 e_phoff;
    struct sizetypes                 e_shoff;
    struct sizetypes                 e_flags;
    struct sizetypes                 e_ehsize;
    struct sizetypes                 e_phentsize;
    struct sizetypes                 e_phnum;
    struct sizetypes                 e_shentsize;
    struct sizetypes                 e_shnum;
    struct sizetypes                 e_shstrndx;
};


#endif
