#include <stdint.h>
#include "bitops.h"
#include "image.h"
#include "data.h"
#include "encoder.h"

int imgarc_encode(int16_t *sequence, imgarc_data *data, imgarc_image *img, uint8_t verbose)
{
	int n = 0, s = 0, current_channel, current_bit_offset;
	int16_t cs;
	uint8_t current_byte = 0, current_bit, current_pixel_channel_val;
	int current_pixel_count = 0;
	int pm = (data->size * 0.1);
	if (verbose == 1)
		printf("Progress: ");

	if ((data->size * 8) > (img->width * img->height))
		return -1;

	while (n < data->size)
	{		
		current_byte = data->data[n];		
		for (int8_t cbc=7; cbc>=0; cbc--)
		{
			cs = sequence[s];
			
			//Get bit.
			current_bit = imgarc_get_bit_8(&current_byte, cbc);
			
			//Set the bit of the relevant pixel.
			current_channel = imgarc_encoder_get_pixel_channel(cs);
			current_bit_offset = imgarc_encoder_get_bit_offset(cs);
			//printf("Encoding bit %i on channel %i\n", current_bit_offset, current_channel);

			current_pixel_channel_val = imgarc_image_get_pixel_channel_value(img, current_pixel_count, current_channel);
			//printf("Pixel %i's %ith channel value is %i\n", current_pixel_count, current_channel, current_pixel_channel_val);

			if (current_bit == 0)
				imgarc_clear_bit_8(&current_pixel_channel_val, current_bit_offset);
			else
				imgarc_set_bit_8(&current_pixel_channel_val, current_bit_offset);

			//Write the new value.
			imgarc_image_set_pixel_channel_value(img, current_pixel_count, current_channel, current_pixel_channel_val);

			s++;
			if (sequence[s] == -1)
				s = 0;
			
			current_pixel_count++;
		}
		n++;

		if (n % pm == 0 && verbose == 1)
			printf("▓▓");

	}

	if (verbose == 1)
		printf("\n");

	return 1;

}

int imgarc_decode_to_data(imgarc_data *obj, int16_t *sequence, imgarc_image *img, uint8_t verbose)
{
	int n = 0, s = 0, data_start_index;
	uint8_t current_byte, file_name_length;
	uint8_t size_bytes[4] = {0};
	/*uint8_t checksum[20] = {0};
	uint8_t filename[255] = {0};*/

	//Read first four bytes, get size.
	s = imgarc_decode_read_n_bytes_from_img(size_bytes, sequence, img, 0, 4, s);
	uint32_t size = imgarc_bytes_to_uint32(size_bytes);

	//printf("Size: %u bytes\n", size);

	uint8_t *data = malloc(size * sizeof(uint8_t));
	s = 0;
	s = imgarc_decode_read_n_bytes_from_img(data, sequence, img, 0, size, s);

	//Validate data first?
	

	obj->size = size;
	obj->data = data;
	
	//Read checksum.
	/*
	s = imgarc_decode_read_n_bytes_from_img(checksum, sequence, img, 4, 20, s);

	//Read file name.
	s = imgarc_decode_read_n_bytes_from_img(filename, sequence, img, 24, 255, s);
	for (int i=0; i<255; i++)
	{
		if (filename[i] == '\0')
		{
			file_name_length = i;
			break;
		}
	}

	data_start_index = 4 + 20 + file_name_length;*/
	
	
	
		/*
		00 00 01 B4 C2 1F 1F C0 39 3A E1 7C EF BE BA 54 1B 21 87 99 6F E8 30 06 62 69 74 6F 70 73 2E 63 00 23 69 6E 63 6C 75 64 65 20 3C 73 74 64 69 6E 74 2E 68 3E 0A 0A 76 6F 69 64 20 69 6D 67 61 72 63 5F 73 65 74 5F 62 69 74 5F 38 28 75 69 6E 74 38 5F 74 20 2A 64 61 74 61 2C 20 75 69 6E 74 38 5F 74 20 62 69 74 5F 6E 29 0A 7B 0A 09 2A 64 61 74 61 20 7C 3D 20 31 20 3C 3C 20 62 69 74 5F 6E 3B 0A 7D 0A 0A 76 6F 69 64 20 69 6D 67 61 72 63 5F 63 6C 65 61 72 5F 62 69 74 5F 38 28 75 69 6E 74 38 5F 74 20 2A 64 61 74 61 2C 20 75 69 6E 74 38 5F 74 20 62 69 74 5F 6E 29 0A 7B 0A 09 2A 64 61 74 61 20 26 3D 20 7E 28 31 20 3C 3C 20 62 69 74 5F 6E 29 3B 0A 7D 0A 0A 75 69 6E 74 38 5F 74 20 69 6D 67 61 72 63 5F 67 65 74 5F 62 69 74 5F 38 28 63 6F 6E 73 74 20 75 69 6E 74 38 5F 74 20 2A 64 61 74 61 2C 20 75 69 6E 74 38 5F 74 20 62 69 74 5F 6E 29 0A 7B 0A 09 72 65 74 75 72 6E 20 28 2A 64 61 74 61 20 3E 3E 20 62 69 74 5F 6E 29 20 26 20 30 78 30 31 3B 0A 7D 0A 0A 75 69 6E 74 33 32 5F 74 20 69 6D 67 61 72 63 5F 62 79 74 65 73 5F 74 6F 5F 75 69 6E 74 33 32 28 75 69 6E 74 38 5F 74 20 2A 62 79 74 65 73 29 0A 7B 0A 09 75 69 6E 74 33 32 5F 74 20 72 65 73 75 6C 74 20 3D 20 30 3B 0A 09 66 6F 72 20 28 69 6E 74 20 6E 3D 30 3B 20 6E 3C 34 3B 20 6E 2B 2B 29 0A 09 09 72 65 73 75 6C 74 20 3D 20 28 72 65 73 75 6C 74 20 3C 3C 20 38 29 20 2B 20 62 79 74 65 73 5B 6E 5D 3B 0A 09 0A 09 72 65 74 75 72 6E 20 72 65 73 75 6C 74 3B 0A 7D
		
		*/

	return 1;

}

int imgarc_decode_read_n_bytes_from_img(uint8_t *storage, int16_t *sequence, imgarc_image *img, int start_from, int n_bytes, int s)
{
	int n = start_from, current_channel, current_bit_offset, end_at = start_from + n_bytes;
	int16_t cs;
	uint8_t current_byte = 0, current_bit, current_pixel_channel_val;
	int current_pixel_count = n * 8;
	uint32_t result = 0;
	int idx = 0;

	while (n < end_at)
	{		
		current_byte = 0;
		for (int8_t cbc=7; cbc>=0; cbc--)
		{
			cs = sequence[s];
			
			//Set the bit of the relevant pixel.
			current_channel = imgarc_encoder_get_pixel_channel(cs);
			current_bit_offset = imgarc_encoder_get_bit_offset(cs);
			current_pixel_channel_val = imgarc_image_get_pixel_channel_value(img, current_pixel_count, current_channel);
			
			//Pul relevant bit.
			current_bit = imgarc_get_bit_8(&current_pixel_channel_val, current_bit_offset);
			//printf("%i", current_bit);

			//Push onto byte stack.
			current_byte = (current_byte << 1) + current_bit;

			s++;
			if (sequence[s] == -1)
				s = 0;
			
			current_pixel_count++;
		}
		//printf("\n%02X\n", current_byte);
		storage[idx] = current_byte;
		n++;
		idx++;
		
	}

	return s;

}

int imgarc_encoder_get_pixel_channel(int16_t current_sequence)
{
	uint8_t bit, cs = current_sequence;

	/**
	 * 000 - channel 2
	 * 001 - channel 1
	 * 011 - channel 0
	 * 010 - channel 0
	*/
	
	bit = imgarc_get_bit_8(&cs, 1);
	if (bit == 1)
		return 2;
	return imgarc_get_bit_8(&cs, 0);
	
	return 0;
}

int imgarc_encoder_get_bit_offset(int16_t current_sequence)
{
	uint8_t cs = current_sequence;

	/**
	 * 0xx - bit 0
	 * 1xx - bit 1
	*/
	
	return (imgarc_get_bit_8(&cs, 0) == 1 ? 0 : 1);
}
