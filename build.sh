#!/bin/bash
mkdir -p bin
cd src
gcc *.c -o ../bin/imgarc
chmod +x ../bin/imgarc
cd ../