CC = gcc
AR = ar
CFLAGS = -std=c99 -pedantic -Wall -fPIC -D_GNU_SOURCE -O2 -I../
ARFLAGS = rcs
LDFLAGS = -lX11 -lXext -lrt
SRC = os.c winsys.c window.c surface.c xwinsys.c clock.c
OBJ = os.o winsys.o window.o surface.o xwinsys.o clock.o

all: libeuropa.a libeuropa.so

libeuropa.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

libeuropa.so: $(OBJ)
	$(CC) -shared $^ -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f libeuropa.a libeuropa.so $(OBJ)

.PHONY: clean
