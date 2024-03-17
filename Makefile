
CFILES = $(wildcard *.c)
OBJ = $(CFILES:.c=.o)
OUT = wcc

CC = clang
CFLAGS = -Wall $(shell pkg-config --cflags libzip)
LDFLAGS = $(shell pkg-config --libs libzip)

$(OUT): $(OBJ)

.PHONY: clean
clean:
	rm -f $(OBJ) $(OUT)
