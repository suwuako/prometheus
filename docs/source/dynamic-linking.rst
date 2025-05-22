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
*n* programs, there would be a statically generated binary copy of ``printf``! Now,
extrapolate that for other cstdlib functions: ``malloc``, ``free``, ``putc``, ``memcpy``.
That would mean having a copy of each file would take up so much space!!!! What do we do?

Dynamically linked files
*********************

Here comes dynamically linked files: 


References
**********
https://medium.com/@bdov_/https-medium-com-bdov-c-dynamic-libraries-what-why-and-how-66cf777019a7
