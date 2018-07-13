#include <stdint.h>

#define imgarc_set_bit_8(a, b)	*a |= 1 << b
#define imgarc_clear_bit_8(a, b) *a &= ~(1 << b)
#define imgarc_get_bit_8(a, b) ((*a >> b) & 0x01)

extern inline uint32_t imgarc_bytes_to_uint32(uint8_t *bytes)
{
	uint32_t result = 0;
	for (int n=0; n<4; n++)
		result = (result << 8) + bytes[n];
	
	return result;
}