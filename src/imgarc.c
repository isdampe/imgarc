#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
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

void imgarc_usage()
{
	printf("Usage: imgarc [mode] [files]\n");
	printf("Encoding: imgarc -e -i example.png -f source_file.txt -p password -o output.png\n");
	printf("Decoding: imgarc -d -i output.png -p password\n");
	exit(1);
}

int imgarc_do_encode(char *img_fp, char *file_fp, char *password, char *out_fp, bool verbose)
{
	imgarc_image img_src;
	uint32_t max_file_size = 0;

	//Read the file to be encoded.
	imgarc_file fd = imgarc_read_file(file_fp);
	if (fd.size_bytes < 1)
		imgarc_fatal("cannot read input_file, or the file is too small.");
	
	//Set data for this file.
	imgarc_data obj = imgarc_data_from_fd(&fd);
	
	if (verbose)
	{
		printf("-- INPUT FILE --\n");
		imgarc_io_print_file_meta(&fd);
		printf("\n");
	}
	
	//Set password sequence.
	int16_t *sequence = imgarc_pass_to_sequence(password);

	//Read the image.
	imgarc_image_read_png_from_file(img_fp, &img_src);
	//if (img_src.error > 0)
	//	imgarc_fatal("Error reading png from file.");

	if (verbose)
	{
		printf("-- SOURCE IMAGE FILE --\n");
		imgarc_io_print_image_png_meta(&img_src);
		printf("\n");
	}

	max_file_size = imgarc_image_get_max_encoding_size_bytes(&img_src);
	if (obj.size >= max_file_size)
	{
		printf("The maximum file size that can be encoded into this PNG is %u bytes.\n", max_file_size);
		imgarc_fatal("Input file exceeded the maximum encodable file size.");
	}

	if (verbose)
	{
		printf("-- ENCODING SEQUENCE --\n");
		imgarc_io_print_sequence(sequence);
		printf("\n");
		printf("-- BEGIN ENCODING --\n");
	}
	
	if (imgarc_encode(sequence, &obj, &img_src, verbose) < 0)
		imgarc_fatal("Error encoding data into PNG. Is the file too big?");
	
	if (verbose)
	{
		printf("Encoding complete.\n\n");
		printf("-- OUTPUT FILE --\n");
	}
	
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
}

int imgarc_do_decode(char *img_fp, char *password, char *out_dir, bool verbose)
{
	imgarc_image img_src;
	imgarc_data obj;
	imgarc_file new_file;
	int result;
	int16_t *sequence = imgarc_pass_to_sequence(password);

	//Read the imageš.
	imgarc_image_read_png_from_file(img_fp, &img_src);
	if (img_src.error > 0)
		imgarc_fatal("Error reading png from file.");

	if (verbose)
	{
		printf("-- SOURCE IMAGE FILE --\n");
		imgarc_io_print_image_png_meta(&img_src);
		printf("\n");
	}
	
	result = imgarc_decode_to_data(&obj, sequence, &img_src, 1);

	new_file = imgarc_file_from_data(&obj);
	if (verbose)
	{
		printf("-- DECODED FILE --\n");
		imgarc_io_print_file_meta(&new_file);
	}

	//Write the file.
	if (! imgarc_data_write_file(&obj, &new_file, out_dir, verbose))
		printf("An error occured when writing %s\n", new_file.name);

	//Clean up.
	free(sequence);
	imgarc_image_png_free(&img_src);
	imgarc_data_free(&obj);
	imgarc_free_file(&new_file);
	
	return 0;
}

int main(int argc, char **argv)
{
	char *img_fp = NULL, *in_fp = NULL, *password = NULL, *out_fp = NULL;
	enum {MODE_INVALID, MODE_ENCODE, MODE_DECODE} mode = MODE_INVALID;
	int c;
	bool verbose = false;

	opterr = 0;

	while ((c = getopt (argc, argv, "edvi:f:p:o:")) != -1)
	{
		switch (c)
		{
			case 'v':
				verbose = true;
			break;
			case 'e':
				mode = MODE_ENCODE;
			break;
			case 'd':
				mode = MODE_DECODE;
			break;
			case 'i':
				img_fp = optarg;
			break;
			case 'f':
				in_fp = optarg;
			break;
			case 'p':
				password = optarg;
			break;
			case 'o':
				out_fp = optarg;
			break;
			case '?':
				if (optopt == 'i' || optopt == 'f' || optopt == 'p' || optopt == 'o')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			break;
			default:
				imgarc_usage();
		}
	}

	switch (mode)
	{
		case MODE_INVALID:
			imgarc_usage();
		break;
		case MODE_ENCODE:
			if (
				img_fp == NULL ||
				in_fp == NULL ||
				password == NULL ||
				out_fp == NULL
			)
				imgarc_usage();
			
			return imgarc_do_encode(img_fp, in_fp, password, out_fp, verbose);
		break;
		case MODE_DECODE:
			if (
				img_fp == NULL ||
				password == NULL
			)
				imgarc_usage();
			if (out_fp == NULL)
				out_fp = "";
			return imgarc_do_decode(img_fp, password, out_fp, verbose);
		break;
	}

	return 0;
	
}
