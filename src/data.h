#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "file_io.h"

typedef struct
{
	uint8_t *data;
	uint32_t size;
} imgarc_data;

void imgarc_data_from_fd(imgarc_data *obj, imgarc_file *fd);
void imgarc_data_free(imgarc_data *obj);
void imgarc_file_from_data(imgarc_file *fd, const imgarc_data *obj);
bool imgarc_data_write_file(imgarc_data *obj, imgarc_file *file, char *file_dir, bool verbose);