#include <stdint.h>

void imgarc_set_bit_8(uint8_t *data, uint8_t bit_n);
void imgarc_clear_bit_8(uint8_t *data, uint8_t bit_n);
uint8_t imgarc_get_bit_8(const uint8_t *data, uint8_t bit_n);
uint32_t imgarc_bytes_to_uint32(uint8_t *bytes);