CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -I../
LDFLAGS = -L../ -l:libeuropa.a -lX11

all: clean window_test.out surface_test.out

window_test.out: window_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

surface_test.out: surface_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f window_test.out surface_test.out

.PHONY: clean