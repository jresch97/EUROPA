CC = gcc
AR = ar
CFLAGS = -std=c99 -pedantic -Wall -fPIC -I../
ARFLAGS = rcs
LDFLAGS = -lX11
SRC = window.c surface.c surface_xlib.c window_win32.c window_xlib.c
OBJ = window.o surface.o surface_xlib.o window_win32.o window_xlib.o

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