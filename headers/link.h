#ifndef LINK_H
#define LINK_H

#include "obj_metadata.h"

enum st_info_binding {
    LOCAL, GLOBAL, WEAK
};

enum st_info_type {
    FUNC, OBJECT, SECTION
};

void validate_objmeta(Objdata *head, Args args);
void stb_global_collision(Hashmap **file_symbols, Hashmap **globals, Objdata *filedata, Args args);
bool shared_emachine(uint64_t base, uint64_t input);

#endif
