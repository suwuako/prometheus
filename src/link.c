#include "../headers/link.h"
#include "../headers/hashmap.h"
#include "../headers/dump_symbol_table.h"

// for linking to succeed:
//
// ensure no global bindings are shared between files
// undefined external references (every symbol needs to have a corresponding address)
void validate_objmeta(Objdata *head, Args args) {
    // ensure no global overlap
    Objdata *curr = head;
    Hashmap **globals = hashmap_new();

    while (curr != NULL) {
        Elf_header eheader = curr->eheader;
        Section_header *sheader = curr->sheaders;
        char *path = curr->path;
        Hashmap **curr_symtab = curr->symtab;

        grab_symbol_names(eheader, sheader, args, path);
        

        curr = curr->next;
    }
}

