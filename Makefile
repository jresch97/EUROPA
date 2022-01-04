CC = gcc
AR = ar
CFLAGS = -std=c99 -pedantic -Wall -fPIC -I../
ARFLAGS = rcs
LDFLAGS = -lX11 -lXext
SRC = os.c winsys.c window.c surface.c hwsurface.c xlibwinsys.c
OBJ = os.o winsys.o window.o surface.o hwsurface.o xlibwinsys.o

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