#include "util.h"

uint64_t get_range_bit(uint64_t word, uint8_t lbit, uint8_t fbit)
{
	const uint64_t mask = (1ull << (lbit - fbit + 1)) - 1;
	const uint64_t val = word >> (fbit - 1);
	return val & mask;
}

void set_range_bit(uint64_t *word, uint8_t lbit, uint8_t fbit, uint8_t val)
{
	const uint64_t mask = (1ull << (lbit - fbit + 1)) - 1;
	if (val == 1)
		*word |= (mask << (fbit - 1));
	else
		*word &= ~(mask << (fbit - 1));
}

void copy_range_bit(uint64_t *word, uint8_t lbit, uint8_t fbit, uint64_t val)
{
	printf("*word=%lu, lbit=%u, fbit=%u, val=%u\n", *word, lbit, fbit, val);
	const uint64_t mask = val << (fbit - 1);
	printf("mask=%x\n", mask);
	set_range_bit(word, lbit, fbit, 0);
	*word |= mask;
}
