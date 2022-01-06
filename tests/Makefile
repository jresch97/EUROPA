CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -I../
LDFLAGS = -L../ -l:libeuropa.a -lm -lX11 -lXext

all: clean europa os_tests.out window_tests.out surface_tests.out hwsurface_tests.out

europa:
	cd ../ && make && cd -

os_tests.out: os_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

window_tests.out: window_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

surface_tests.out: surface_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

hwsurface_tests.out: hwsurface_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f os_tests.out window_tests.out surface_tests.out hwsurface_tests.out

.PHONY: clean