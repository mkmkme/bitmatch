#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

inline uint8_t get_hex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c + 10 - 'a';
	assert(0);
}


inline uint8_t get_bit(uint64_t word, uint8_t nbit)
{
	return (word >> (nbit - 1)) & 1;
}

uint64_t get_range_bit(uint64_t word, uint8_t lbit, uint8_t fbit);
void set_range_bit(uint64_t *word, uint8_t lbit, uint8_t fbit, uint8_t val);
void copy_range_bit(uint64_t *word, uint8_t lbit, uint8_t fbit, uint64_t val);


#endif
