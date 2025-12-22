#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../headers/dump_elf_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/parse_args.h"
#include "../headers/misc.h"

void fatal_error(char *error_message) {
    fprintf(stderr, error_message);
    fprintf(stderr, "\n");
    exit(1);
}

bool file_readable(char *path) {
    if (access(path, R_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

bool file_exists(char *path) {
    if (access(path, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

void print_args(Args args) {
    printf("arg flags:\n");
    printf("- dump header: %d\n", args.dump_header);

    printf("- file:\n");
    printf("\t- filename: %s\n", args.path.filepath);
    printf("\t- set: %d\n", args.path.set);
    printf("\t- len: %d\n", args.path.size);
}

// reads nbytes before we have parsed an elf header
uint64_t read_nbytes(FILE *fd, Elf_header *header, int byte_count, bool variable) {
    uint64_t ret = 0;

    if (variable) {
        if (header->ei_class == 1) {
            byte_count = 4;
        } else {
            byte_count = 8;
        }
    }

    // little endian
    if (header->ei_data == 1) {
        ret |= getc(fd);
        ret |= ((uint64_t) getc(fd) << (8));
        if (byte_count == 2) { goto READ_NBYTES__END; }
        
        ret |= ((uint64_t) getc(fd) << (8 * 2));
        ret |= ((uint64_t) getc(fd) << (8 * 3));
        if (byte_count == 4) { goto READ_NBYTES__END; }
        
        ret |= ((uint64_t) getc(fd) << (8 * 4));
        ret |= ((uint64_t) getc(fd) << (8 * 5));
        ret |= ((uint64_t) getc(fd) << (8 * 6));
        ret |= ((uint64_t) getc(fd) << (8 * 7));
        if (byte_count == 8) { goto READ_NBYTES__END; }
    } else if (header->ei_data == 2) {
        ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        if (byte_count == 4) { goto READ_NBYTES__END; }

        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        if (byte_count == 4) { goto READ_NBYTES__END; }

        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        if (byte_count == 8) { goto READ_NBYTES__END; }
    } else {
        fatal_error("ERROR: expected endianess to be 1 or 2.");
    }

READ_NBYTES__END:
    return ret;
}

// bytes indicates how many bytes to read. 0 if is a variable value (32/64 bits)
// factors in little/big endian.
uint64_t read_nbytes_better(Elf_header header, FILE *fd, int bytes, bool variable) {
    uint64_t ret = 0;
    if (variable) {
        if (header.ei_class == THIRTY_TWO_BIT) {
            // if 32 bit
            bytes = VARIABLE_32BIT_SIZE;
        } else {
            bytes = VARIABLE_64BIT_SIZE;
        }
    }

    if (header.ei_data == LITTLE_ENDIAN_CUSTOM) {
        for (int i = 0; i < bytes; i++) {
            ret |= (fgetc(fd) << (8 * i));
        }
    } else if (header.ei_data == BIG_ENDIAN_CUSTOM) {
        for (int i = 0; i < bytes; i++) {
            ret |= fgetc(fd); ret >>= 8;
        }
    }

    return ret;
}

// debug function to dump nbytes from a file offset.
void DEBUG_DUMP_NBYTES(int offset, int n, Args args) {
    FILE *fd = fopen(args.path.filepath, "r");

    lseek(fileno(fd), offset, SEEK_SET);
    for (int i = 0 ; i < n; i++) {
        if (i % 16 == 0) {
            printf("\n");
        } if (i % 2 == 0) {
            printf(" ");
        }
        printf("%02x", fgetc(fd));
    }
    fclose(fd);
}

void read_stream_until_null(FILE *fd) {
    char result[100];
    int i = 0;

    // read string until null term
    while ((result[i] = fgetc(fd)) != '\0') i++;

    printf(SH_NAME_ALIGN_STRING, result);
}
