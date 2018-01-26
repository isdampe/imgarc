#include <stdint.h>
#include "bitops.h"
#include "image.h"
#include "data.h"
#include "encoder.h"

int imgarc_encode(int16_t *sequence, imgarc_data *data, imgarc_image *img)
{
	int n = 0, s = 0, current_channel, current_bit_offset;
	int16_t cs;
	uint8_t current_byte = 0, current_bit, current_pixel_channel_val;
	int current_pixel_count = 0;

	if ((data->size * 8) > (img->width * img->height))
		return -1;

	while (n < data->size)
	{		
		current_byte = data->data[n];
		//printf("%02X: ", current_byte);
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

	}

	return 1;

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


