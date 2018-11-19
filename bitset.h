#ifndef BITSET_H
#define BITSET_H

#include <stdint.h>

struct bitset {
	uint64_t length;
	uint64_t cells;
	uint64_t *data;
};

int bitset_init(struct bitset *bs, uint64_t length);
void bitset_free(struct bitset *bs);
void bitset_set_bits(struct bitset *bs, uint64_t lbit, uint64_t fbit, uint64_t val);
void bitset_shift_with_bit(struct bitset *bs, uint8_t bit);

#endif // BITSET_H
