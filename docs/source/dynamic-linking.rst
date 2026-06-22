Dynamic Linking
###############

Why do we need dynamic linking?
************************

So far, we've only covered static linking: we take data from .o (object files) and
substitute them in the binary of other files. That means that your compiled ``a.out`` file
will contain the physical binary code of every single statically linked function!
But lets take a look at say, ``printf``.
``printf`` itself relies on different parts of the C standard library, and is used almost
everywhere!! If every single program on your computer used ``printf``, that would mean for
*n* programs, there would be a statically generated binary copy of ``printf`` *n* times! Now,
extrapolate that for other cstdlib functions: ``malloc``, ``free``, ``putc``, ``memcpy``.
That would mean having a copy of each file would take up so much space!!!! What do we do?

Dynamically linked files
*********************

Here comes dynamically linked files: They're functionally the same as static linking;
the 'promise' of the symbols still exist within the your binary ``a.out``. The compiler
and linker still ensures that everything is resolved properly and your code will run without
a hitch. But when you run the program, a dynamic loader will then link your ``a.out`` binary
with the shared library before loading it into memory!

We are reusing our example code from background:

``print.c:``

.. code-block:: C 
   :linenos:

    #include <stdio.h>

    void print_me() {
        printf("meow!\n");
    }


``call_print.c:``

.. code-block:: C
   :linenos:

    void print_me();

    int main() {
        print_me();
    }

Now, let's dynamically compile!!!

First, we need to turn ``print.c`` into position independent code (PIC). As per its name,
PIC code is code that runs properly regardless of where its address is. This is opposed to 
absolute code, which is code that **NEEDS** to be loaded in a specific location to function
properly. PIC is usually used by shared libraries, so that the same code can be loaded into
different program's address spaces without causing overlap in memory. The way PIC code 
differs from position dependent code (PDC) is with the ways jumps are made: PIC code does
relative jumps; jumps are made from the current position (jump 300 bytes forwards from
current position), whereas PDC code does absolute jumps (jump to address 0xFABC1234).

As a result, we can see why we need to compile our shared ``print.o`` object file as PIC
code if we want it to be a shared library!

.. code-block:: bash

    ~/temp/linking ❯ ls
    call_print.c  print.c

    ~/temp/linking ❯ gcc -c -fpic print.c -o print.o

Next, we need to turn it into an actual shared library using the ``-shared`` flag in gcc!
Looking online, I got vague answers on what the flag actually does [4]. Most of it appears
to be with how flags for link time are set, alongside how stdin is handled. For our purposes,
we just need to know that do some magic flag stuff to make linking nicer for us :)

.. code-block:: bash

    ~/temp/linking ❯ gcc -o libprint.so -shared print.o

Now we *try* to compile the actual binary with the shared library! keep in mind that elf
dynamic libraries need to be called some variant of ``libxxxxx.so``. This is so that we can
use the ``-lprint`` flag, which tries to find the shared library ``libprint.so`` in a
specified directory. the ``-L`` flag lets you specify which directories to look for these
shared libraries in!

.. code-block:: bash

    ~/temp/linking ❯ gcc -L/home/suwa/temp/linking -o a.out call_print.c -lprint
    ~/temp/linking ❯ ls
    a.out  call_print.c  call_print.o  libprint.so  print.c  print.o
    ~/temp/linking ❯ ./a.out                                                                                     git 01:13:15 PM
    ./a.out: error while loading shared libraries: libprint.so: cannot open shared object file: No such file or directory

It compiles successfully! But why can't we run it? This is because we need to actually add
the directory that contains the shared library to PATH so that the loader knows where to 
actually find ``libprint.so``! 


.. code-block:: bash

    ~/temp/linking ❯ WD="$(pwd)"                                                                                 git 01:16:14 PM

    ~/temp/linking ❯ echo $WD                                                                                    git 01:16:23 PM
    /home/suwa/temp/linking

    ~/temp/linking ❯ echo $LD_LIBRARY_PATH                                                                       git 01:16:26 PM

    ~/temp/linking ❯ LD_LIBRARY_PATH=$WD:$LD_LIBRARY_PATH                                                        git 01:17:13 PM

    ~/temp/linking ❯ echo $LD_LIBRARY_PATH                                                                       git 01:17:43 PM
    /home/suwa/temp/linking:
    ~/temp/linking ❯ export LD_LIBRARY_PATH

meowmeow you can see we now added ts, confirm if it works

.. code-block:: bash

    ~/temp/linking ❯ ./a.out                                                                                     git 01:19:26 PM
    meow!

wow it works

.. code-block:: bash
    ~/temp/linking ❯ ldd a.out                                                                                   git 09:40:51 AM
        linux-vdso.so.1 (0x00007f23f2232000)
        libprint.so => not found
        libc.so.6 => /usr/lib/libc.so.6 (0x00007f23f2004000)
        /lib64/ld-linux-x86-64.so.2 => /usr/lib64/ld-linux-x86-64.so.2 (0x00007f23f2234000)

References
**********
0. https://medium.com/@bdov_/https-medium-com-bdov-c-dynamic-libraries-what-why-and-how-66cf777019a7

1. https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

2. https://en.wikipedia.org/wiki/Position-independent_code

3. https://stackoverflow.com/questions/14680867/what-is-the-difference-between-position-dependent-code-and-position-independent

4. https://stackoverflow.com/questions/25084855/how-does-gcc-shared-option-affect-the-output
