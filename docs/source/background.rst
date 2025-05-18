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

.. code-block::
    $ gcc call_print.c
    /usr/bin/ld: /tmp/cc5dtrRn.o: in function ``main':
    call_print.c:(.text+0x5): undefined reference to ``print_me'
    collect2: error: ld returned 1 exit status

This is obvious: we try to call a function that has a **declaration** that it exists,
but there is no **definition** for ``print_me`` in ``call_print.c``. The declaration only
exists in ``print.c``. As a result, we are trying to call code that doesn't exist! Lets try
to compile this time with ``print.c`` as an argument for gcc.

.. code-block::
    ~/temp/linking ❯ gcc print.c call_print.c

    ~/temp/linking ❯ ./a.out
    meow!

It compiles and links properly! So what actaully happened in between? We can take a
closer look by telling gcc to compile both ``print.c`` and ``call_print.c`` as object files
using the ``-c`` flag.

.. code-block::
    ~/temp/linking ❯ gcc -c print.c call_print.c

    ~/temp/linking ❯ ls
    call_print.c  call_print.o  print.c  print.o

We can see that we have two new files: ``call_print.o`` and ``print.o``. These are the object
files of their respective names. Lets take a look inside them to see what they hold. 
