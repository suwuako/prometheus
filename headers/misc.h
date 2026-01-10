#ifndef MISC_H
#define MISC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdnoreturn.h>

#include "dump_elf_header.h"
#include "parse_args.h"


bool is_32_bit(Elf_header header);
noreturn void fatal_error(char *error_message);
bool file_exists(char *path);
bool file_readable(char *path);
void print_args(Args args);
uint64_t read_nbytes(FILE *fd, Elf_header *header, int byte_count, bool variable);
uint64_t read_nbytes_better(Elf_header header, FILE *fd, int bytes, bool variable);
void DEBUG_DUMP_NBYTES(int offset, int n, Args args);
void read_stream_until_null(FILE *fd);

#endif
