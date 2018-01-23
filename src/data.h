#pragma once
#include <stdint.h>
#include "file_io.h"

typedef struct
{
	uint8_t *data;
	uint32_t size;
} imgarc_data;

imgarc_data imgarc_data_from_fd(imgarc_file *fd);
void imgarc_data_free(imgarc_data *obj);
imgarc_file imgarc_file_from_data(const imgarc_data *obj);