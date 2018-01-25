#include <stdio.h>
#include "file_io.h"
#include "data.h"
#include "user_io.h"
#include "password.h"
#include "image.h"

void imgarc_fatal(char *error)
{
	printf("FATAL: %s\n", error);
	exit(1);
}

void usage()
{
	imgarc_fatal("Usage: imgarc image.png input_file password output_file");
}

int main(int argc, char **argv)
{
	if (argc < 5)
		usage();
	
	char *img_fp = argv[1];
	char *in_fp = argv[2];
	char *password = argv[3];
	char *out_fp = argv[4];

	int16_t *sequence = imgarc_pass_to_sequence(password);

	imgarc_file fd = imgarc_read_file(in_fp);
	if (fd.size_bytes < 1)
		imgarc_fatal("cannot read input_file, or it's too small.");

	imgarc_data obj = imgarc_data_from_fd(&fd);
	imgarc_io_print_file_meta(&fd);
	imgarc_io_print_sequence(sequence);

	imgarc_file new_fd = imgarc_file_from_data(&obj);
	//imgarc_io_print_file_meta(&new_fd);
	
	//Clear memory.
	free(sequence);
	imgarc_free_file(&fd);
	imgarc_data_free(&obj);
	imgarc_free_file(&new_fd);

	//Read the image.
	imgarc_image img_src;
	printf("Opening %s\n", img_fp);
	imgarc_image_read_png_from_file(img_fp, &img_src);
	if (img_src.error > 0)
		imgarc_fatal("Error reading png from file.");
	
	imgarc_io_print_image_png_meta(&img_src);

	//Encode the data.

	//Write the output file.

	imgarc_image_png_free(&img_src);
	
	return 0;
}