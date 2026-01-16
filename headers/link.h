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

#endif
