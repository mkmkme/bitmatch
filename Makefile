CFLAGS = -Wall -Wextra -pedantic -O3
TGT = bitmatch

all: $(TGT)

clean:
	rm -f $(TGT)

test: $(TGT)
	# given test case
	echo -n 'h>0?' | ./$(TGT) f8c 11
	# look for 'ELF' in the executable
	cat ./$(TGT) | ./$(TGT) 454c46 24
	# first implementation will fail here
	cat t.bin | ./$(TGT) db4 11


.PHONY: clean
