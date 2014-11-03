language0
=========
language0 is a very simple, maybe the most simple programming language. This project demonstrates how to create a lexer, parser, optimization passes and differend back ends for a programming language. The code in this project could serve as a basis for a full-fledged compiler.


Getting started
---------------
* `make language0` to create the compiler.
* `./language0 cc0 examples/fibonacci.lang0 fibonacci.as0` to compile the fibonacci example into language0 assembler.
* `./language0 as0 fibonacci.as0 fibonacci.bin0` to compile the fibonacci assembler program into byte code.
* `./language0 run fibonacci.bin0 --` to run the byte code and print the results to stdout.


Front End
---------
A simple, recursive descent parser that is able to parse the grammar that is specified in `language0.g` (ANTLR format).


Checks and optimization passes
------------------------------
Since language0 does not allow functions / function calls and knows only one data type (32 bit signed integer), the only necessary check is that variable names are not specified more than once and only used if declared.

Implemented optimizations:
* *constant folding* turning i.e. source code like `print 1 + 2` into `print 3`.
* *dead code removal* to remove i.e. `if(false){...}` completely.


Back Ends
---------
This project contains several backends:
* *cc0* compiles language0 source code into language0 assembler.
* *dot* compiles language0 source code into a GraphViz graph (call `dot -Tpng INPUT -o OUTPUT` to generate a picture).
* *g32* compiles language0 source code into x86_32 assembly suitable to be compiled by gcc (call `gcc -m32 -xassembler INPUT -o OUTPUT` to generate an executable file).


Virtual Machine
---------------
The assembler demonstrates a simple two-pass assembler. In the first pass, symbols are read and their address is calculated, the second pass actually emits the program.

The virtual machine itself is a simple, stack based design. Memory access is bound checked.


License
-------
(c) 2014 Tim Wiederhake, licensed under GPLv3 or leater
