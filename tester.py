#!/usr/bin/env python3

import argparse
from bitarray import bitarray
import sys


def hex_to_bita(arg):
    a = bitarray(bin(int(arg, 16))[2:])
    if a.length() % 4 != 0:
        a = bitarray('0' * (4 - (a.length() % 4))) + a
    return a


p = argparse.ArgumentParser()
p.add_argument('pattern', type=hex_to_bita, help='pattern to look for')
p.add_argument('len', type=int, help='length of the pattern')
p.add_argument('--search', action='store_true', default=False,
    help='bit search max length of suitable pattern')
args = p.parse_args()


inp = bitarray()
inp.frombytes(sys.stdin.buffer.read())

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
