#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "file_io.h"


void imgarc_read_file(imgarc_file *fd, char *fp)
{
	FILE *fh;
	int n = 0;

	fd->size_bytes = 0;

	fh = fopen(fp, "rb");
	if (! fh)
		return;

	//Get size in bytes.
	fseek(fh, 0, SEEK_END);
	fd->size_bytes = ftell(fh);
	fseek(fh, 0, SEEK_SET);

	//Filename.
	char *bn = basename(fp);
	size_t sl = strlen(bn) +1;
	fd->name = malloc(sl * sizeof(char));
	strncpy(fd->name, bn, sl -1);
	fd->name[sl -1] = '\0';

	//Alocate space.
	fd->data = malloc(fd->size_bytes * sizeof(uint8_t));

	while (n < fd->size_bytes)
	{
		fseek(fh, n, SEEK_SET);
		fread(&fd->data[n], 1, 1, fh);
		++n;
	}

	//Checksum.
	SHA1_CTX ctx;
	sha1_init(&ctx);
	sha1_update(&ctx, fd->data, fd->size_bytes);
	sha1_final(&ctx, fd->checksum);
}

void imgarc_free_file(imgarc_file *fd)
{
	free(fd->name);
	if (fd->size_bytes > 0)
		free(fd->data);
}

bool imgarc_file_verify_checksum(imgarc_file *fd)
{
	uint8_t checksum[SHA1_BLOCK_SIZE];

	SHA1_CTX ctx;
	sha1_init(&ctx);
	sha1_update(&ctx, fd->data, fd->size_bytes);
	sha1_final(&ctx, checksum);

	for (int i=0;i<SHA1_BLOCK_SIZE;i++)
		if (checksum[i] != fd->checksum[i])
			return false;

	return true;

}
