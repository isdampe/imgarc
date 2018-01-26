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
