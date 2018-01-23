#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "file_io.h"


imgarc_file imgarc_read_file(char *fp)
{
	FILE *fh;
	imgarc_file fd;
	int n = 0;

	fd.size_bytes = 0;
	
	fh = fopen(fp, "rb");
	if (! fh)
		return fd;
	
	//Get size in bytes.
	fseek(fh, 0, SEEK_END);
	fd.size_bytes = ftell(fh);
	fseek(fh, 0, SEEK_SET);

	//Filename.
	char *bn = basename(fp);
	size_t sl = strlen(bn) +1;
	fd.name = malloc(sl * sizeof(char));
	strncpy(fd.name, bn, sl -1);

	//Alocate space.
	fd.data = malloc(fd.size_bytes * sizeof(uint8_t));

	while (n < fd.size_bytes)
	{
		fseek(fh, n, SEEK_SET);
		fread(&fd.data[n], 1, 1, fh);
		++n;
	}

	//Perform a hash?

	return fd;
}

void imgarc_free_file(imgarc_file *fd)
{
	free(fd->name);
	if (fd->size_bytes > 0)
		free(fd->data);
}