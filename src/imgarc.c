#include <stdio.h>
#include "file_io.h"
#include "data.h"
#include "user_io.h"
#include "password.h"
#include "image.h"
#include "encoder.h"

void imgarc_fatal(char *error)
{
	printf("FATAL: %s\n", error);
	exit(1);
}

void usage()
{
	printf("Usage: imgarc image.png input_file password output_file\n");
	imgarc_fatal("Usage: imgarc image.png password output_file");
}


int main(int argc, char **argv)
{
	if (argc < 4)
		usage();
	
	char *img_fp = argv[1];
	char *password = argv[2];
	char *out_fp = argv[3];

	int16_t *sequence = imgarc_pass_to_sequence(password);

	//Read the image.
	imgarc_image img_src;
	imgarc_image_read_png_from_file(img_fp, &img_src);
	if (img_src.error > 0)
		imgarc_fatal("Error reading png from file.");

	printf("-- SOURCE IMAGE FILE --\n");
	imgarc_io_print_image_png_meta(&img_src);
	printf("\n");

	imgarc_data obj;
	int result = imgarc_decode_to_data(&obj, sequence, &img_src, 1);

	imgarc_file new_file = imgarc_file_from_data(&obj);
	imgarc_io_print_file_meta(&new_file);

	//Clean up.
	free(sequence);
	imgarc_image_png_free(&img_src);
	
	return 0;
}

/*
int main(int argc, char **argv)
{
	if (argc < 5)
		usage();
	
	char *img_fp = argv[1];
	char *in_fp = argv[2];
	char *password = argv[3];
	char *out_fp = argv[4];
	uint32_t max_file_size = 0;

	//Read the file to be encoded.
	imgarc_file fd = imgarc_read_file(in_fp);
	if (fd.size_bytes < 1)
		imgarc_fatal("cannot read input_file, or it's too small.");
	
	//Set data for this file.
	imgarc_data obj = imgarc_data_from_fd(&fd);
	
	printf("-- INPUT FILE --\n");
	imgarc_io_print_file_meta(&fd);
	printf("\n");

	//Set password sequence.
	int16_t *sequence = imgarc_pass_to_sequence(password);

	//Read the image.
	imgarc_image img_src;
	imgarc_image_read_png_from_file(img_fp, &img_src);
	if (img_src.error > 0)
		imgarc_fatal("Error reading png from file.");

	printf("-- SOURCE IMAGE FILE --\n");
	imgarc_io_print_image_png_meta(&img_src);
	printf("\n");

	max_file_size = imgarc_image_get_max_encoding_size_bytes(&img_src);
	if (obj.size >= max_file_size)
	{
		printf("The maximum file size that can be encoded into this PNG is %u bytes.\n", max_file_size);
		imgarc_fatal("Input file exceeded the maximum encodable file size.");
	}

	printf("-- ENCODING SEQUENCE --\n");
	imgarc_io_print_sequence(sequence);
	printf("\n");

	printf("-- BEGIN ENCODING --\n");
	if (imgarc_encode(sequence, &obj, &img_src, 1) < 0)
		imgarc_fatal("Error encoding data into PNG. Is the file too big?");
	
	printf("Encoding complete.\n\n");

	printf("-- OUTPUT FILE --\n");
	imgarc_image_write_png(out_fp, &img_src);
	if (img_src.error > 0)
		imgarc_fatal("Error writing png to file.");
	
	printf("Wrote encoded PNG to %s.\nCleaning up.\n", out_fp);

	//Clean up.
	free(sequence);
	imgarc_free_file(&fd);
	imgarc_data_free(&obj);
	imgarc_image_png_free(&img_src);

	printf("Encoding done.\n");
	
	return 0;
}*/