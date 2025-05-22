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

First, we need to turn ``print.c`` into a shared library:

.. code-block:: bash

    ~/temp/linking ❯ ls
    call_print.c  print.c

    ~/temp/linking ❯ gcc -c -fpic print.c -o print.o

blah blah fpic code somethign here
Then, we need to convert it into an actual shared liba blahb labha

.. code-block:: bash

    ~/temp/linking ❯ gcc -o libprint.so -shared print.o

fds

.. code-block:: bash

    ~/temp/linking ❯ gcc -L/home/suwa/temp/linking -o a.out call_print.c -lprint
    ~/temp/linking ❯ ls
    a.out  call_print.c  call_print.o  libprint.so  print.c  print.o
    ~/temp/linking ❯ ./a.out                                                                                     git 01:13:15 PM
    ./a.out: error while loading shared libraries: libprint.so: cannot open shared object file: No such file or directory

blabla doesnt know where to look for libprint

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


References
**********
https://medium.com/@bdov_/https-medium-com-bdov-c-dynamic-libraries-what-why-and-how-66cf777019a7

https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html
