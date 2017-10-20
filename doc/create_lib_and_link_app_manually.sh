#!/bin/bash

# Create library
g++ -c -o lib.o lib/source/functions.cpp -Ilib/include/
ar cr liblib.a lib.o

# Create app using library
g++ -c -o main.o app/source/main.cpp -Ilib/include/
g++ -o app.exe main.o -L./ -llib