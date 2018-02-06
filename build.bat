mkdir bin
cd src
gcc -o ../bin/imgarc.exe *.c -lpng
cd ../
echo You'll need to copy libpng3.dll to bin/ now
echo Then, you can run with bin/imgarc.exe
