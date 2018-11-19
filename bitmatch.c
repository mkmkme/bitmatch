#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bitset.h"
#include "util.h"

struct input_stream {
	struct bitset set;
	uint8_t bit_buf;
	uint8_t idx;
};

static inline int input_stream_init(struct input_stream *is, uint64_t numbits)
{
	is->idx = 0;
	if (bitset_init(&(is->set), numbits) < 0)
		return -1;
	read(STDIN_FILENO, is->set.data, is->set.cells);
	return 0;
}

static inline int input_stream_next_bit(struct input_stream *is)
{
	if (is->idx == 0) {
		if (!read(STDIN_FILENO, &is->bit_buf, 1))
			return -1;
		is->idx = 8;
	}
	bitset_shift_with_bit(&is->set, get_bit(is->bit_buf, is->idx));
	--is->idx;
	return 0;
}

static void fill_arg_bitset(struct bitset *bs, const char *str)
{
	int i;
	size_t chr = 0; // number of the symbol in the string
	for (i = bs->length; i >= 4; i -= 4)
		bitset_set_bits(bs, i, i - 4 + 1, get_hex(str[chr++]));
	if (i > 0)
		bitset_set_bits(bs, i, 1, get_hex(str[chr]) >> (4 - i));
}

int main(int argc, char *argv[])
{
	char *endptr = NULL;
	uint64_t total_bits;
	struct bitset arg_set;
	struct input_stream is;

	if (argc != 3) {
		fprintf(stderr, "usage: %s PATTERN NBITS\n", argv[0]);
		return 2;
	}

	total_bits = strtoull(argv[2], &endptr, 10);
	if (endptr == NULL || endptr == argv[2] || *endptr != '\0') {
		printf("bad nbits '%s'\n", argv[2]);
		return 2;
	}

	if (bitset_init(&arg_set, total_bits) < 0) {
		printf("cannot init arg bit set\n");
		return 2;
	}
	fill_arg_bitset(&arg_set, argv[1]);
	if (input_stream_init(&is, total_bits) < 0) {
		printf("can't init input stream\n");
		return 2;
	}

	for (;;) {
		if (memcmp(is.set.data, arg_set.data, arg_set.cells * sizeof(uint64_t)) == 0)
			break;
		if (input_stream_next_bit(&is) < 0) {
			bitset_free(&arg_set);
			bitset_free(&is.set);
			return 1;
		}
	}
	bitset_free(&arg_set);
	bitset_free(&is.set);
	return 0;
}

