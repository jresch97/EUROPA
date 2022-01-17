CC = gcc
AR = ar
FT = -I/usr/include/freetype2
PNG = -I/usr/include/libpng16
CFLAGS = -std=c99 -pedantic -Wall -fPIC -D_GNU_SOURCE -O3 -I../ $(FT) $(PNG)
ARFLAGS = rcs
LDFLAGS = -lX11 -lXext -lrt -lfreetype
SRC = os.c winsys.c window.c surface.c xwinsys.c clock.c font.c ftfontsys.c
OBJ = os.o winsys.o window.o surface.o xwinsys.o clock.o font.o ftfontsys.o

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
