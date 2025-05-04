#include <stdbool.h>
#include <stdio.h>

#include "consts.h"

bool check_magic(FILE *obj) {
    const int ELF_MAGIC[ELF_MAGIC_SIZE] = {0x7f, 'E', 'L', 'F'};
    for (int i = 0; i < ELF_MAGIC_SIZE; i++) {
        if (fgetc(obj) != ELF_MAGIC[i]) { return false; }
    }
    return true;
}
