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
