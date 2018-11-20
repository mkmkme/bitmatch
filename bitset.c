#include "bitset.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int bitset_init(struct bitset *bs, uint64_t length)
{
	bs->cells = ceil(length * 1. / 64.);
	bs->data = calloc(bs->cells, sizeof(uint64_t));
	if (bs->data == NULL)
		return -1;
	bs->length = length;
	return 0;
}

void bitset_free(struct bitset *bs)
{
	free(bs->data);
}

void bitset_set_bits(struct bitset *bs, uint64_t lbit, uint64_t fbit, uint64_t val)
{
	const uint64_t lcell = (lbit-1) / 64; // cell containing last bit
	const uint64_t fcell = (fbit-1) / 64; // cell containing first bit

	if (lcell == fcell) {
		DBG("lbit=%lu, fbit=%lu, val=%lu, lcell=%lu\n", lbit - 64 * lcell, fbit - 64 * lcell, val, lcell);
		copy_range_bit(&bs->data[lcell], lbit - 64 * lcell, fbit - 64 * lcell, val);
	} else {
		// 5...11
		//       3 1   8  5
		// 1101 1111 | 1000 1100
		//       ___ | ____
		const uint8_t total_bits_to_send = lbit - fbit + 1; // 11 - 5 + 1 = 7
		// numbers of bits in their cells
		const uint8_t lbitcell = lbit % 64; // 3
		const uint8_t fbitcell = fbit % 64; // 5
		// lbit for first bit
		const uint8_t fpartmaxidx = 64 - fbitcell + 1; // 8 - 5 + 1 = 4
		const uint64_t fbitdata = get_range_bit(val, fpartmaxidx, 1);
		const uint64_t lbitdata = get_range_bit(val, total_bits_to_send, fpartmaxidx + 1);
		copy_range_bit(&bs->data[lcell], lbitcell, 1, lbitdata);
		copy_range_bit(&bs->data[fcell], 64, fbitcell, fbitdata);
	}
}

void bitset_shift_with_bit(struct bitset *bs, uint8_t bit)
{
	size_t i;
	uint64_t lbit = bit;
	for (i = 0; i < bs->cells; ++i) {
		uint64_t t = get_bit(bs->data[i], 64);
		bs->data[i] = (bs->data[i] << 1) | lbit;
		lbit = t;
	}
	// now truncate the first word
	bitset_set_bits(bs, (uint64_t)ceil(bs->length / 64.) * 64, bs->length + 1, 0);
}

#ifdef DEBUG_PRINT
static inline void print_bin(uint64_t x)
{
	int i;
	for (i = 0; i < 64; i++)
		putchar(((x >> (63 - i)) & 1) ? '1' : '0');
	putchar('\n');
}

void bitset_print(struct bitset *bs, const char *pref)
{
	uint64_t i;
	printf("%s: ", pref);
	for (i = 0; i < bs->cells; ++i)
		print_bin(bs->data[i]);
}
#endif
