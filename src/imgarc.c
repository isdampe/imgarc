#include <stdio.h>
#include "file_io.h"
#include "data.h"

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

	imgarc_file fd = imgarc_read_file(in_fp);
	if (fd.size_bytes < 1)
		imgarc_fatal("cannot read input_file, or it's too small.");

	imgarc_data obj = imgarc_data_from_fd(&fd);

	imgarc_file new_fd = imgarc_file_from_data(&obj);
	printf("File: %s\nSize: %i bytes\nChecksum: %s\n", new_fd.name, new_fd.size_bytes, new_fd.checksum);
	
	imgarc_free_file(&fd);
	imgarc_data_free(&obj);
	imgarc_free_file(&new_fd);
	return 0;
}