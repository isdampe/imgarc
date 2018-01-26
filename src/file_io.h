#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "sha1.h"

typedef struct
{
	char *name;
	uint32_t size_bytes;
	uint8_t *data;
	uint8_t checksum[SHA1_BLOCK_SIZE];
} imgarc_file;

imgarc_file imgarc_read_file(char *fp);
void imgarc_free_file(imgarc_file *fd);
int imgarc_write_file(const imgarc_file *file);
bool imgarc_file_verify_checksum(imgarc_file *fd);