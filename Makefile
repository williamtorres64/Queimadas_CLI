CC = gcc
CFLAGS = -I. -Isrc -Icsv -g3 -O0
LDFLAGS = -lncursesw -lm
SRC := $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
TARGET = queimadas_cli

.PHONY: all clean prod

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

prod:
	$(MAKE) CFLAGS="-I. -Isrc -Icsv -O3 -march=native -flto -pipe -fomit-frame-pointer" LDFLAGS="$(LDFLAGS) -s" all

exe:
	$(MAKE) CC=x86_64-w64-mingw32-gcc TARGET=queimadas_cli.exe LDFLAGS="-Lsrc/PDCurses/wincon -lpdcurses -lm" CFLAGS="-I. -Isrc -Icsv -g3 -O0 -DPDC_WIDE=1 -D_GNU_SOURCE" all

clean:
	rm -f $(OBJ) $(TARGET)

