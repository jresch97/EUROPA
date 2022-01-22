CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -O3 -I../
LDFLAGS = -L../ -l:libeuropa.a -lm -lX11 -lXext -lrt -lfreetype

all: clean europa os_tests.out window_tests.out surface_tests.out clock_tests.out font_tests.out

europa:
	cd ../ && make clean && make && cd -

os_tests.out: os_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

window_tests.out: window_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

surface_tests.out: surface_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clock_tests.out: clock_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

font_tests.out: font_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f os_tests.out window_tests.out surface_tests.out clock_tests.out font_tests.out

.PHONY: clean
