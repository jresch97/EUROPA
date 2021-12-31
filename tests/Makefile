CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -I../
LDFLAGS = -L../ -l:libeuropa.a -lX11

all: europa_test.out

europa_test.out: europa_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f europa_test.out

.PHONY: clean