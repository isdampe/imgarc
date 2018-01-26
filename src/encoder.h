#include <stdint.h>
#include "image.h"
#include "data.h"

int imgarc_encode(int16_t *sequence, imgarc_data *data, imgarc_image *img, uint8_t verbose);
int imgarc_encoder_get_pixel_channel(int16_t current_sequence);
int imgarc_encoder_get_bit_offset(int16_t current_sequence);