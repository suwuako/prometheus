#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"
#include "../headers/obj_metadata.h"

int main(int argc, char *argv[]) {
    printf("== hello, prom == \n\n");
    Args args = parse_args(argc, argv);
    print_args(args);
    Objdata *obj = get_objdata(&args);
    dump_objdata(obj, args);

    return 0;
}
