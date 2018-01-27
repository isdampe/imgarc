#!/bin/bash
mkdir -p bin
cd src
gcc *.c -o ../bin/imgarc -lpng16
chmod +x ../bin/imgarc
cd ../