#!/usr/bin/env python3

import argparse
from bitarray import bitarray
import sys


def hex_to_bita(arg):
    a = bitarray(len(arg) * 4)

    def get_bitarray_from_chr(c, pos):
        if '0' <= c <= '9':
            return bitarray(format(ord(c) - ord('0'), '04b'))
        if 'a' <= c <= 'f':
            return bitarray(format(ord(c) + 10 - ord('a'), '04b'))
        raise argparse.ArgumentTypeError('bad sym %s in pos %d' % (c, pos))

    for num, sym in enumerate(arg):
        pos = num * 4
        a[pos:pos + 4] = get_bitarray_from_chr(sym, num)

    return a


p = argparse.ArgumentParser()
p.add_argument('pattern', type=hex_to_bita, help='pattern to look for')
p.add_argument('len', type=int, help='length of the pattern')
p.add_argument('--search', action='store_true', default=False,
    help='bit search max length of suitable pattern')
args = p.parse_args()


inp = bitarray()
for byte in sys.stdin.buffer.read():
    c = bitarray(format(byte, '08b'))
    inp.extend(c)


print('input has %d bits' % inp.length())
print('input = %s' % inp)
print('arg has %d bits' % args.pattern.length())
args.pattern = args.pattern[:args.len]
print('after trim arg pattern has %d bits' % args.pattern.length())
print('pattern = %s' % args.pattern)
maxlen = args.len
if args.search:
    while args.len > 4:
        args.pattern = args.pattern[:maxlen]
        if args.pattern in inp:
            print('found! length = %d' % maxlen)
            sys.exit(0)
        maxlen -= 1
    print('not found at all')
    sys.exit(1)
else:
    print(args.pattern in inp)
