#include <stdio.h>
#include "file_io.h"
#include "sha1.h"
#include "image.h"

void imgarc_io_print_file_meta(const imgarc_file *fd)
{
	printf("File name: %s\n", fd->name);
	printf("Data size: %i bytes\n", fd->size_bytes);
	printf("Checksum: ");
	for (int i=0; i<SHA1_BLOCK_SIZE; i++)
		printf("%02X ", fd->data[i]);
	printf("\n");
}

void imgarc_io_print_sequence(const int16_t *sequence)
{
	int i = 0; int16_t c;
	printf("Sequence: ");
	while (1)
	{
		c = sequence[i];
		if (c == -1)
			break;
		++i;
		printf("%i ", c);
		
	}
	printf("\n");
}


void imgarc_io_print_image_png_meta(imgarc_image *img)
{
	printf("Bit depth: %i\n", img->bit_depth);
	printf("Width: %i pixels\n", img->width);
	printf("Height: %i pixels\n", img->height);
}