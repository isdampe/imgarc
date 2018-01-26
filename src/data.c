#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "data.h"
#include "file_io.h"
#include "sha1.h"

imgarc_data imgarc_data_from_fd(imgarc_file *fd)
{
	imgarc_data obj;
	size_t name_length;
	int n = 0, a = 0;

	name_length = strlen(fd->name) +1;

	obj.size = (
		4 //The 4 bytes representing the data size
		+ fd->size_bytes //The actual data size.
		+ name_length //The length of the file name.
		+ SHA1_BLOCK_SIZE //For checksum.
	);
	
	obj.data = malloc(obj.size * sizeof(uint8_t));
	
	//Push each byte from the size_bytes into the set.
	//Using shifts keeps it portable.
	for (n=0; n<4; n++)
		obj.data[a + n] = (fd->size_bytes >> (32 - (8 * (n +1)))) & 0xFF;
	a = a+n;	

	for (n=0; n<SHA1_BLOCK_SIZE; n++)
		obj.data[a + n] = fd->checksum[n];
	a = a+n;
	
	for (n=0; n<name_length; n++)
		obj.data[a + n] = fd->name[n];
	a = a+n;
	
	for (n=0; n<fd->size_bytes; n++)
		obj.data[a + n] = fd->data[n];

	return obj;

}

void imgarc_data_free(imgarc_data *obj)
{
	free(obj->data);
}

imgarc_file imgarc_file_from_data(const imgarc_data *obj)
{
	imgarc_file fd;
	int n = 0, a = 0;
	char tmp_filename[256];

	fd.size_bytes = 0;
	
	//Shift bytes into uint32_t.
	for (n=0; n<4; n++)
		fd.size_bytes = (fd.size_bytes + obj->data[a + n]) << (n < 3 ? 8 : 0);
	a = a + n;
	
	//Read checksum.
	for (n=0; n<SHA1_BLOCK_SIZE; n++)
		fd.checksum[n] = obj->data[a + n];
	a = a + n;
	
	//Read filename.
	for (n=0; n<255; n++)
	{
		tmp_filename[n] = obj->data[a + n];
		if (tmp_filename[n] == '\0')
			break;
	}
	a = a + (n +1); //An additional +1 to compensate the \0 of file name.
	
	//Allocate space for name.
	fd.name = malloc((n +1) * sizeof(char));
	strncpy(fd.name, tmp_filename, n +1);

	//Allocate space for data.
	fd.data = malloc(fd.size_bytes * sizeof(uint8_t));

	//Read from next until n + size (as read in first step)
	for (n=0; n<fd.size_bytes; n++)
		fd.data[n] = obj->data[a + n];
	
	return fd;
	
}

bool imgarc_data_write_file(imgarc_data *obj, imgarc_file *file, char *file_dir, bool verbose)
{
	char out_fp[255];
	int file_dir_length = strlen(file_dir);

	//Setup the output file path for writing.
	strcpy(out_fp, file_dir);
	if (file_dir_length > 0)
	if (out_fp[strlen(file_dir) -1] != '/')
		strcat(out_fp, "/");
	strcat(out_fp, file->name);

	if (verbose)
		printf("Writing %i bytes to %s\n", obj->size, out_fp);

	FILE *fp = fopen(out_fp, "wb");
	if (! fp)
		return false;

	if (fwrite(file->data, sizeof(uint8_t), file->size_bytes, fp) == 0)
		return false;
	
	return true;
}