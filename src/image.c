#include <stdlib.h>
#include <png.h>
#include "image.h"

void imgarc_image_read_png_from_file(const char *image_fp, imgarc_image *img)
{
	FILE *fp = fopen(image_fp, "rb");

	img->png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (! img->png)
	{
		img->error = IMGARC_IMG_ERROR_READ_STRUCT;
		return;
	}

	img->info = png_create_info_struct(img->png);
	if (! img->info)
	{
		img->error = IMGARC_IMG_ERROR_CREATE_INFO;
		return;
	}

	if (setjmp(png_jmpbuf(img->png)))
	{
		img->error = IMGARC_IMG_ERROR_SETJMP;
		return;
	}

	png_init_io(img->png, fp);
	png_read_info(img->png, img->info);

	img->width = png_get_image_width(img->png, img->info);
	img->height = png_get_image_height(img->png, img->info);
	img->color_type = png_get_color_type(img->png, img->info);
	img->bit_depth  = png_get_bit_depth(img->png, img->info);

	//Ensure 16 bit PNG's are interpreted as 8 bit.
	if (img->bit_depth == 16)
		png_set_strip_16(img->png);

	//Ensure we use RGB.
	if (img->color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(img->png);

	//We really do want an 8 bit PNG
	if (img->color_type == PNG_COLOR_TYPE_GRAY && img->bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(img->png);

	if (png_get_valid(img->png, img->info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(img->png);

	//Ensure all colors types have an alpha channel set to 0xff.
	if (
		img->color_type == PNG_COLOR_TYPE_RGB ||
		img->color_type == PNG_COLOR_TYPE_GRAY ||
		img->color_type == PNG_COLOR_TYPE_PALETTE
	)
		png_set_filler(img->png, 0xff, PNG_FILLER_AFTER);

	//Convert GRAY's to rgb.
	if (
		img->color_type == PNG_COLOR_TYPE_GRAY ||
		img->color_type == PNG_COLOR_TYPE_GRAY_ALPHA
	)
		png_set_gray_to_rgb(img->png);

	png_read_update_info(img->png, img->info);

	img->row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * img->height);
	for (int y=0; y<img->height; y++) {
		img->row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(img->png, img->info));
	}

	png_read_image(img->png, img->row_pointers);

	fclose(fp);

}

void imgarc_image_png_free(imgarc_image *img)
{
	for (int y=0; y<img->height;y++)
		free(img->row_pointers[y]);
	free(img->row_pointers);
	png_destroy_read_struct(&img->png, &img->info, NULL);
}

uint8_t imgarc_image_get_pixel_channel_value(imgarc_image *img, int pixel_number, int channel)
{
	int y = (pixel_number / img->width), x = (pixel_number % img->width);
	png_bytep row = img->row_pointers[y];
	png_bytep px = &(row[x * 4]);
	//printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
	return px[channel];
}

void imgarc_image_set_pixel_channel_value(imgarc_image *img, int pixel_number, int channel, uint8_t value)
{
	int y = (pixel_number / img->width), x = (pixel_number % img->width);
	png_bytep row = img->row_pointers[y];
	png_bytep px = &(row[x * 4]);
	px[channel] = value;
}

long imgarc_image_get_max_encoding_size_bytes(const imgarc_image *img)
{
	return ((img->width * img->height) / 8) - 20 - 4 - 255;
}

void imgarc_image_write_png(const char *output_fp, imgarc_image *img)
{
	int y;

	FILE *fp = fopen(output_fp, "wb");
	if(! fp)
	{
		img->error = IMGARC_IMG_ERROR_FWRITE;
		return;
	}

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (! png)
	{
		img->error = IMGARC_IMG_ERROR_WRITE_STRUCT;
		return;
	}

	png_infop info = png_create_info_struct(png);
	if (! info)
	{
		img->error = IMGARC_IMG_ERROR_CREATE_INFO;
		return;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		img->error = IMGARC_IMG_ERROR_SETJMP;
		return;
	}

	png_init_io(png, fp);

	//Output format.
	png_set_IHDR(
		png,
		info,
		img->width, 
		img->height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(png, info);

	png_write_image(png, img->row_pointers);
	png_write_end(png, NULL);
	fclose(fp);
	png_destroy_write_struct(&png, &info);

}
