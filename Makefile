CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

nncc: $(OBJS)
	$(CC) -o nncc $(OBJS) $(LDFLAGS)

all: nncc

test: nncc
	./test/test.sh

clean:
	rm -f nncc *.o *~ tmp*

.PHONY: test clean
