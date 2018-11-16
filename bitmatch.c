#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct input_bitstream {
	uint8_t buf;
	int8_t idx;
};

static inline uint8_t get_hex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c + 10 - 'a';
	assert(0);
}

static inline uint8_t get_bit(uint8_t word, uint8_t nbit)
{
	return (word >> nbit) & 1;
}

static inline void input_bitstream_init(struct input_bitstream* bs)
{
	bs->idx = -1;
}

static int input_bitstream_next_bit(struct input_bitstream* bs, uint8_t* ret_b)
{
	if (bs->idx < 0) {
		if (!read(0, &bs->buf, 1))
			return -1;
		bs->idx = 7;
	}
	*ret_b = get_bit(bs->buf, bs->idx);
	--bs->idx;
	return 0;
}

static inline uint8_t pattern_get_bit(const char *str, uint64_t bit)
{
	const uint8_t byte = get_hex(str[bit / 4]);
	return get_bit(byte, 3 - bit % 4);
}

int main(int argc, char *argv[])
{
	const char *pattern;
	char *endptr = NULL;
	uint64_t total_bits;
	struct input_bitstream ibs;

	if (argc != 3) {
		fprintf(stderr, "usage: %s PATTERN NBITS\n", argv[0]);
		return 2;
	}

	pattern = argv[1];
	total_bits = strtoull(argv[2], &endptr, 10);
	if (endptr == NULL || endptr == argv[2] || *endptr != '\0') {
		printf("bad nbits '%s'\n", argv[2]);
		return 2;
	}

	input_bitstream_init(&ibs);

	for (;;) {
		uint64_t i = 0;
		uint8_t input;
		for (; i < total_bits; ++i) {
			const uint8_t arg = pattern_get_bit(pattern, i);
			if (input_bitstream_next_bit(&ibs, &input) < 0)
				return 1;
			if (arg != input)
				break;
		}
		if (i == total_bits)
			return 0;
	}
}

