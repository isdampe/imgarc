#pragma once
#include <stdlib.h>

typedef struct
{
	char *name;
	uint32_t size_bytes;
	uint8_t *data;
	char checksum[64];
} imgarc_file;

imgarc_file imgarc_read_file(char *fp);
void imgarc_free_file(imgarc_file *fd);
int imgarc_write_file(const imgarc_file *file);
