# imgarc - Encode, hide and protect binary files in PNG images.

imgarc is a simplistic steganographic encoder/decoder. You can use it to encode, hide and protect
and kind of binary file in a PNG image. You can then decode the PNG image to retrieve the original binary file.

While imgarc does use a "password" for encoding and decoding, it _does not_ encrypt data. It is instead used to
scramble the order in which your data is stored within the PNG image. If your data is considered sensitive,
you should encrypt it before encoding it, and then decrypt it after decoding it.

## Requirements

* libpng16
* gcc

## Compiling

```bash
cd imgarc/
./build.sh
```

## Example usage

```bash
cd imgarc/
# Encode src/file_io.c into example.png and store it in output.png
./bin/imgarc -e -i example.png -f src/file_io.c -p password123 -o output.png -v
# Decode output.png and store the file it contains in /tmp
./bin/imgarc -d -i output.png -p password123 -o /tmp -v
# Show the file output after decoding.
cat /tmp/file_io.c
```