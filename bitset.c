#include "bitset.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int bitset_init(struct bitset *bs, uint64_t length)
{
	bs->cells = ceil(length * 1. / 64.);
	printf("cells = %lu\n", bs->cells);
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
	const uint64_t lcell = lbit / 64; // 1
	const uint64_t fcell = fbit / 64; // 0

	if (lcell == fcell) {
		printf("lcell==fcell\n");
		copy_range_bit(&bs->data[lcell], lbit, fbit, val);
		return;
	}

	// 5...11
	//       3 1   8  5
	// 1101 1111 | 1000 1100
	//       ___ | ____
	const uint8_t total_bits_to_send = lbit - fbit + 1; // 11 - 5 + 1 = 7
	const uint8_t lbitcell = lbit % 64; // 3
	const uint8_t fbitcell = fbit % 64; // 5
	const uint8_t fpartmaxidx = 64 - fbitcell + 1; // 8 - 5 + 1 = 4
	const uint64_t fbitdata = get_range_bit(val, fpartmaxidx, 1);
	const uint64_t lbitdata = get_range_bit(val, total_bits_to_send, fpartmaxidx + 1);
	copy_range_bit(&bs->data[lcell], lbitcell, 1, lbitdata);
	copy_range_bit(&bs->data[fcell], 64, fbitcell, fbitdata);
}

static void print_bin(uint64_t word)
{
	for (int j = 0; j < 64; ++j)
		putchar(((word >> (63 - j)) & 1) ? '1' : '0');
	putchar('\n');
}

void bitset_shift_with_bit(struct bitset *bs, uint8_t bit)
{
	size_t i = 0;
	uint8_t lbit = bit;
	uint64_t mask;
	// printf("shifting with bit %u\n", bit);
	for (; i < bs->cells; ++i) {
		bs->data[i] = (bs->data[i] << 1) | lbit;
		lbit = get_bit(bs->data[i], 8);
	}
	// now truncate the first word
	// printf("truncating to %d bits\n", bs->length % 64);
	mask = (1 << (bs->length % 64)) - 1;
	bs->data[0] &= mask;
}

void bitset_shift_right(struct bitset *bs, uint8_t count)
{
	uint64_t tmp_data = 0;
	int i = bs->cells - 1;
	for (; i >= 0; --i) {
		uint64_t t =  get_range_bit(bs->data[i], count, 1);
		bs->data[i] = (bs->data[i] >> count) | (tmp_data << count);
		tmp_data = t;
	}
}

void bitset_print(struct bitset *bs, const char *pref)
{
	printf("%s: ", pref);
	for (uint64_t i = 0; i < bs->cells; ++i)
		print_bin(bs->data[i]);
}
