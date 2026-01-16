#include "../headers/link.h"
#include "../headers/hashmap.h"

// for linking to succeed:
//
// ensure no global bindings are shared between files
// undefined external references (every symbol needs to have a corresponding address)
void validate_objmeta(Objdata *head) {
    // ensure no global overlap
    Objdata *curr = head;
    Hashmap **globals = hashmap_new();

    while (curr != NULL) {
        Hashmap **curr_symtab = curr->symtab;
        

        curr = curr->next;
    }
}

