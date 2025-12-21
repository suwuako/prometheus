#include <stdint.h>
#include <stdio.h>

#include "../headers/misc.h"
#include "../headers/parse_args.h"
#include "../headers/dump_elf_header.h"

const char *osabi_names[19] = {
  "SystemV", "HP_UX", "NetBSD", "Linux", "GNU_Hurd", "Solaris", "AIX",
  "IRIX", "FreeBSD", "Tru64", "Novell_Modesto", "OpenBSD", "OpenVMS",
  "NonStop_Kernel", "AROS", "FenixOS", "Nuxi_CloudABI", "Stratus_Technologies_OpenVOS"
};

const char *object_type_names[5] = {
    "Unknown", "Relocatable", "Executable", "Shared Object", "Core"
};


void check_and_set_magic(FILE *fd, Elf_header *header) {
    header->ei_magic[0] = getc(fd);
    header->ei_magic[1] = getc(fd);
    header->ei_magic[2] = getc(fd);
    header->ei_magic[3] = getc(fd);

}

void dump_header(Elf_header header) {
    printf("\n== elf header dump ==\n");
    printf("\tMAGIC:                            \t%x, %s\n", header.ei_magic[0], &header.ei_magic[1]);
    printf("\tformat:                           \t%d bits\n", (header.ei_class == 1) ? 32 : 64);
    printf("\tbyte order:                       \t%s endian\n", (header.ei_data == 1) ? "little" : "big");
    printf("\tei_ver:                           \t%d (should be 1)\n", header.ei_version);
    printf("\tosabi:                            \t%s\n", osabi_names[header.ei_osabi]);
    printf("\tabiversion+:                      \t%d\n", header.ei_abiversion);
    printf("\ttype:                             \t%s\n", (header.e_type <= 0x04) ? object_type_names[header.e_type] : "Reserved Other");
    printf("\ttarget ISA:                       \t%s, %ld\n", (header.e_machine) == 0x3e ? "AMD x86-64" : "not amd x86", header.e_machine);
    printf("\teversion:                         \t0x%lx\n", header.e_version);
    printf("\tprog entry address:               \t0x%lx\n", header.e_entry);
    printf("\tprog header address:              \t%ld (bytes into file)\n", header.e_phoff);
    printf("\tsection header offset:            \t%ld (bytes into file)\n", header.e_shoff);
    printf("\tflags:                            \t0x%ld\n", header.e_flags);
    printf("\theader size:                      \t%ld (bytes)\n", header.e_ehsize);
    printf("\tprogram header entry size:        \t%ld (bytes)\n", header.e_phentsize);
    printf("\tprogram header entry num:         \t%ld\n", header.e_phnum);
    printf("\tsection header entry size:        \t%ld (bytes)\n", header.e_shentsize);
    printf("\tsection header entry num:         \t%ld\n", header.e_shnum);
    printf("\tsection header string table index:\t%ld\n", header.e_shstrndx);
}

Elf_header grab_elf_header(Args args) {
    Elf_header ret = {};
    FILE *fd = fopen(args.path.filepath, "r");
    check_and_set_magic(fd, &ret);
    ret.ei_class = getc(fd);
    ret.ei_data = getc(fd);
    ret.ei_version = getc(fd);
    ret.ei_osabi = getc(fd);
    ret.ei_abiversion = getc(fd);
    for (int i = 0; i < PADDING_SIZE; i++) {
        ret.ei_pad[i] = getc(fd);
    }
    ret.e_type = read_nbytes(fd, &ret, 2, false);
    ret.e_machine = read_nbytes(fd, &ret, 2, false);
    ret.e_version = read_nbytes(fd, &ret, 4, false);
    ret.e_entry = read_nbytes(fd, &ret, 0, true);
    ret.e_phoff = read_nbytes(fd, &ret, 0, true);
    ret.e_shoff = read_nbytes(fd, &ret, 0, true);
    ret.e_flags = read_nbytes(fd, &ret, 4, false);
    ret.e_ehsize = read_nbytes(fd, &ret, 2, false);
    ret.e_phentsize = read_nbytes(fd, &ret, 2, false);
    ret.e_phnum = read_nbytes(fd, &ret, 2, false);
    ret.e_shentsize = read_nbytes(fd, &ret, 2, false);
    ret.e_shnum = read_nbytes(fd, &ret, 2, false);
    ret.e_shstrndx = read_nbytes(fd, &ret, 2, false);

    fclose(fd);
    return ret;
}
