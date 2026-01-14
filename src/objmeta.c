#include <stdlib.h>

#include "../headers/obj_metadata.h"
#include "../headers/parse_args.h"
#include "../headers/misc.h"
#include "../headers/dump_elf_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/dump_program_header.h"
#include "../headers/dump_symbol_table.h"

void obj_clean(Objdata *obj) {
    /*
    if (program_headers != NULL) free(program_headers);
    free(section_headers);
    hashmap_free(symtab);

    */
}

void dump_objdata(Objdata *obj, Args args) {
    Objdata *curr = obj;
    while (curr != NULL) {
        bool proghead_exists = program_header_exists(curr->eheader);
        if (!proghead_exists && args.dump_program_header) fatal_error("-p doesn't work with object files without program headers!");
        if (args.dump_header) dump_header(curr->eheader);
        if (args.dump_section_header) dump_section_headers(curr->sheaders, curr->eheader, args, curr->path);
        if (args.dump_program_header) dump_program_headers(curr->pheaders, curr->eheader, args);
        if (args.dump_symtab) hashmap_visualiser(curr->symtab);
        curr = curr->next;
    }
}

Objdata *get_objdata(Args *args) {
    Filepath *curr = args->path;
    Objdata *head = NULL;

    while (curr != NULL) {
        Elf_header elf_header = grab_elf_header(*args, curr->filepath);
        if (is_32_bit(elf_header)) fatal_error("Linker only will work with 64-bit systems");
        Section_header *section_headers = grab_all_section_headers(elf_header, *args, curr->filepath);
        Program_header *program_headers = NULL;
        bool proghead_exists = program_header_exists(elf_header);
        if (proghead_exists) program_headers = grab_program_headers(elf_header, *args, curr->filepath);
        Hashmap **symtab = grab_symbol_table(elf_header, section_headers, *args, curr->filepath);

        Objdata *curr_obj = head;
        Objdata *new = new_objdata(elf_header, section_headers, program_headers, symtab, curr->filepath);
        if (curr_obj == NULL) {
            head = new;
            curr = curr->next;
            continue;
        }

        while (curr_obj->next != NULL) {
            curr_obj = curr_obj->next;
        }
        curr_obj->next = new;

        curr = curr->next;
    }
    return head;
}

Objdata *new_objdata(Elf_header e, Section_header *s, Program_header *p, Hashmap **sym, char *fn) {
    Objdata *ret = malloc(sizeof(Objdata));

    ret->next = NULL;
    ret->symtab = sym;
    ret->path = fn;
    ret->eheader = e;
    ret->sheaders = s;
    ret->pheaders = p;

    return ret;
}
