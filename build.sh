#!/bin/bash
mkdir -p bin
cd src
gcc -lpng16 *.c -o ../bin/imgarc
chmod +x ../bin/imgarc
cd ../