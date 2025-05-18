Background Information
======================

Compilation
***********

Consider the compilation process for a C file. The file gets parsed through your compiler
of choice, which is then compiled down to assembly. The assembler then takes over, 
creating an object file. A linker then steps in to link the object file against other
binaries to create your program! But how does the linker actually work?

Linker Basics
*************

Lets say we have two files, ``print.c`` and ``call_print.c``:

print.c

.. code-block:: C 
   :linenos:

    #include <stdio.h>

    void print_me() {
        printf("meow!\n");
    }


call_print.c

.. code-block:: C
   :linenos:

    void print_me();

    int main() {
        print_me();
    }


If we were to compile call_print.c by itself, we end up with a linker error:

.. code-block:: Bash

    $ gcc call_print.c
    /usr/bin/ld: /tmp/cc5dtrRn.o: in function ``main':
    call_print.c:(.text+0x5): undefined reference to ``print_me'
    collect2: error: ld returned 1 exit status

This is obvious: we try to call a function that has a **declaration** that it exists,
but there is no **definition** for ``print_me`` in ``call_print.c``. The declaration only
exists in ``print.c``. As a result, we are trying to call code that doesn't exist! Lets try
to compile this time with ``print.c`` as an argument for gcc.

.. code-block:: Bash

    ~/temp/linking ❯ gcc print.c call_print.c

    ~/temp/linking ❯ ./a.out
    meow!

It compiles and links properly! So what actaully happened in between? We can take a
closer look by telling gcc to compile both ``print.c`` and ``call_print.c`` as object files
using the ``-c`` flag.

.. code-block:: Bash

    ~/temp/linking ❯ gcc -c print.c call_print.c

    ~/temp/linking ❯ ls
    call_print.c  call_print.o  print.c  print.o

We can see that we have two new files: ``call_print.o`` and ``print.o``. These are the object
files of their respective names. Lets take a look inside them to see what they hold.

Whats inside an object file?
****************************

We will be using objdump to view the contents of the symbol table with each object file!
We need to specify what we want from the file, which is the contents of the symbol table.
This can be achieved with `-t`:

-t, --syms               Display the contents of the symbol table(s)

.. code-block:: Bash

    ~/temp/linking ❯ objdump -t print.o                                                                          git 01:20:02 AM

    print.o:     file format elf64-x86-64

    SYMBOL TABLE:
    0000000000000000 l    df *ABS*  0000000000000000 print.c
    0000000000000000 l    d  .text  0000000000000000 .text
    0000000000000000 l    d  .rodata        0000000000000000 .rodata
    0000000000000000 g     F .text  0000000000000016 print_me
    0000000000000000         *UND*  0000000000000000 puts



    ~/temp/linking ❯ objdump -t call_print.o                                                                     git 01:20:13 AM

    call_print.o:     file format elf64-x86-64

    SYMBOL TABLE:
    0000000000000000 l    df *ABS*  0000000000000000 call_print.c
    0000000000000000 l    d  .text  0000000000000000 .text
    0000000000000000 g     F .text  0000000000000010 main
    0000000000000000         *UND*  0000000000000000 print_me

Lets unpack this. Lets first look at the objdump spec for symbol entires:

- The first column is the symbol's value (or address offset!).
- The second column (l, g) implies if the current symbol is (l)ocal or (g)lobal.
- The third column (d) implies that its a (d)ebugging symbol.
- The fourth column (f, F) implies if the symbol is a (F)unction or a (f)ile.
- The fifth column (*ABS*, *UND*, .text) implies if the section is absolute (ie not
connected with any section), or *UND* if the section is referenced in the file being
dumped, but not defined there.


Using this, lets then look at ``call_print.o``'s symbols': We can see that there is a symbol
called ``print_me`` that is called! We can then determine that it is a unknown symbol that is
referenced in ``call_print.o`` but has no definition there. If we then look at ``print.o``,
we find that there is a (g)lobal (F)unction called print_me. We can thus infer that
``print_me.o`` creates a temporary header that "promises" the existance of a undefined
symbol somewhere else called ``print_me``. The linker's job is to then find a symbol that
has a matching defined symbol elsewhere to determine what type of symbol it is to link
the two object files together!! Isn't that cool??

References
==========
https://stackoverflow.com/questions/6666805/what-does-each-column-of-objdumps-symbol-table-mean
https://sourceware.org/binutils/docs/binutils/objdump.html#index-symbol-table-entries_002c-printing
