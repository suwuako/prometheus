#ifndef DUMP_HEADER_H
#define DUMP_HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "parse_args.h"
#define THIRTY_TWO_BIT 1
#define SIXTY_FOUR_BIT 2
#define LITTLE_ENDIAN_CUSTOM 1
#define BIG_ENDIAN_CUSTOM 2
#define PADDING_SIZE 7

extern const char *osabi_names[19];
extern const char *object_type_names[5];

typedef struct elf_header {
    char ei_magic[4];                 // elf magic numbers, 0x7f, 'elf'
    uint8_t ei_class;                 // 64/32 bit indicator, 1 for 32, 2 for 64
    uint8_t ei_data;                  // endianess, 1 for little, 2 for big
    uint8_t ei_version;               // set to 1 for original/current elf version
    uint8_t ei_osabi;                 // finds target ABI (0x03 for linux)
    uint8_t ei_abiversion;            // additional abi version
    uint8_t ei_pad[PADDING_SIZE];     // padding bytes
    uint64_t e_type;                  // object file type
    uint64_t e_machine;               // target isa
    uint64_t e_version;               // set to 1 for elf version

                                      // these fields are:
                                      // 32 bit if class == 1
                                      // 64 bits if class == 2
    uint64_t e_entry;                 // entry point of where process starts executing
    uint64_t e_phoff;                 // entry point of program header tabl. 
    uint64_t e_shoff;                 // entry point of section header table

    uint64_t e_flags;                 // target architecture changes interpretation of ts
    uint64_t e_ehsize;                // size of elf header (64/52 bytes if 64/32 bits)
    uint64_t e_phentsize;             // size of program header table entry (0x20 if 32 bit,
                                      // 0x38 if 64 bit)
    uint64_t e_phnum;                 // number of entries in program header table
    uint64_t e_shentsize;             // size of a section header table entry
                                      // (0x28 if 32 bit, 0x40 if 64 bit)
    uint64_t e_shnum;                 // number of entries in section header table
    uint64_t e_shstrndx;              // index of section header table entry that contains
                                      // the section names

} Elf_header;


void dump_header(Elf_header header);
Elf_header grab_elf_header(Args args);
void check_and_set_magic(FILE *fd, Elf_header *header);
uint64_t read_varaible_entries(FILE *fd, Elf_header header);

#endif
