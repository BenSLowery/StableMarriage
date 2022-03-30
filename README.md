Requires C++17 kernel. Works and test on Linux (Ubuntu) and Mac (Mac OS). If wanting to compile the project from source, it may be required to include a makevars file in `~/.R/makevars`. And in there include the lines:

CC=/usr/bin/gcc
CXX=/usr/bin/g++

This is due to some error that I found in compiling from ubuntu and Mac, it didn't really want to use C++17 to compile the project so this forces it to do so.

See PDF for Task 2 and 3.
