#include <stdint.h>

void imgarc_set_bit_8(uint8_t *data, uint8_t bit_n)
{
	*data |= 1 << bit_n;
}

void imgarc_clear_bit_8(uint8_t *data, uint8_t bit_n)
{
	*data &= ~(1 << bit_n);
}

uint8_t imgarc_get_bit_8(const uint8_t *data, uint8_t bit_n)
{
	return (*data >> bit_n) & 0x01;
}

uint32_t imgarc_bytes_to_uint32(uint8_t *bytes)
{
	uint32_t result = 0;
	for (int n=0; n<4; n++)
		result = (result << 8) + bytes[n];
	
	return result;
}