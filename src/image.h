#pragma once
#include <stdint.h>
#include <png.h>

enum imgarc_img_errors
{
	IMGARC_IMG_ERROR_READ_STRUCT,
	IMGARC_IMG_ERROR_CREATE_INFO,
	IMGARC_IMG_ERROR_SETJMP,
	IMGARC_IMG_ERROR_FWRITE,
	IMGARC_IMG_ERROR_WRITE_STRUCT
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
uint8_t imgarc_image_get_pixel_channel_value(imgarc_image *img, int pixel_number, int channel);
void imgarc_image_set_pixel_channel_value(imgarc_image *img, int pixel_number, int channel, uint8_t value);
long imgarc_image_get_max_encoding_size_bytes(const imgarc_image *img);
void imgarc_image_write_png(const char *output_fp, imgarc_image *img);