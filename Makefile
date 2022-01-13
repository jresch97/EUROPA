CC = gcc
AR = ar
CFLAGS = -std=c99 -pedantic -Wall -fPIC -O2 -I../
ARFLAGS = rcs
LDFLAGS = -lX11 -lXext
SRC = os.c winsys.c window.c surface.c xlib_winsys.c
OBJ = os.o winsys.o window.o surface.o xlib_winsys.o

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