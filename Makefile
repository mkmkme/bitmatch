CFLAGS = -Wall -Wextra -pedantic -O3 -std=c99
LDFLAGS = -O3 -lm
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
TGT = bitmatch

all: $(TGT)

$(TGT): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $(TGT)

clean:
	rm -f *.o $(TGT)

$(OBJS): %.o: %.c
	$(CC) -o $*.o -c $(CFLAGS) $<

test: $(TGT)
	# given test case
	echo -n 'h>0?' | ./$(TGT) f8c 11
	# look for 'ELF' in the executable
	cat ./$(TGT) | ./$(TGT) 454c46 24
	# first implementation will fail here
	cat t.bin | ./$(TGT) db4 11


.PHONY: clean
