#pragma once
#include <stdint.h>
#include <png.h>

enum imgarc_img_errors
{
	IMGARC_IMG_ERROR_READ_STRUCT,
	IMGARC_IMG_ERROR_CREATE_INFO,
	IMGARC_IMG_ERROR_SETJMP
};

typedef struct
{
	int8_t error;
	png_structp png;
	png_infop info;
	png_byte color_type;
	png_byte bit_depth;
	int width;
	int height;
	png_bytep *row_pointers;
	
} imgarc_image;

void imgarc_image_read_png_from_file(const char *image_fp, imgarc_image *img);
void imgarc_image_png_free(imgarc_image *img);