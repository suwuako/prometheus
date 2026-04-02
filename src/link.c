#include <elf.h>

#include "../headers/link.h"
#include "../headers/hashmap.h"
#include "../headers/dump_symbol_table.h"
#include "../headers/obj_metadata.h"

bool shared_emachine(uint64_t base, uint64_t input) {
    return base == input;
}

bool etype_is_et_rel(Elf_header *header) {
    return header->e_type == ET_REL;
}

// checks for stb_global symbol collision between files.
// drains the symtab hashmap and inserts into global hashmap
// collisions between equal values will crash
void stb_global_collision(Hashmap **file_symbols, Hashmap **globals, Objdata *filedata, Args args) {
    Hashmap *h_symbol = hashmap_pop(file_symbols);

    while (h_symbol != NULL) {
        char *sym_name = h_symbol->key;
        Elf64_Sym *symbol = h_symbol->entry;

        // check if symbol is global
        if (ELF64_ST_BIND(symbol->st_info) != STB_GLOBAL) {
            h_symbol = hashmap_pop(file_symbols);
            continue;
        }

        // check if global symbol exists
        Hashmap *h;
        if((h = hashmap_find(sym_name, globals)) != NULL) {
            fprintf(stderr, "%s \n%s\n", h->key, sym_name);
            fatal_error("conflicting STB_GLOBAL detected");
        }

        hashmap_visualiser(globals);
        hashmap_insert(sym_name, symbol, globals);
        hashmap_entry_free(h_symbol);
        h_symbol = hashmap_pop(file_symbols);
    }
}

// for linking to succeed:
//
// TODO:
// - [ ] all files are relocatable (eheader->e_type is et_rel)
// - [x] same e_machine
// - [x] ensure no global bindings are shared between files
// undefined external references (every symbol needs to have a corresponding address)
void validate_objmeta(Objdata *head, Args args) {
    printf("Checking std_global conflicts...\n");
    // ensure no global overlap
    Objdata *curr_obj = head;
    Hashmap **globals = hashmap_new();
    uint64_t shared_machine = head->eheader.e_machine;

    while (curr_obj != NULL) {
        Elf_header eheader = curr_obj->eheader;
        Section_header *sheader = curr_obj->sheaders;
        char *path = curr_obj->path;
        Hashmap **curr_symtab = curr_obj->symtab;

        if (!shared_emachine(shared_machine, eheader.e_machine))
            fatal_error("Different e_machine detected");

        if (!etype_is_et_rel(&eheader))
            fatal_error("File's e_type is not ET_REL");

        // we get a copy of allocated symtab because stb_global calls hashmap_pop 
        // which modifies the hashmap.
        Hashmap **symtab_dup = grab_symbol_table(eheader, sheader, args, path);

        printf("adding globals from %s\n", path);
        hashmap_visualiser(globals);
        stb_global_collision(symtab_dup, globals, curr_obj, args);
        hashmap_visualiser(globals);

        curr_obj = curr_obj->next;
    }
}

