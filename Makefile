CC = gcc
CFLAGS = -I. -Isrc -Icsv -g3 -O0
LDFLAGS = -lncursesw -lm
SRC := $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
TARGET = queimadas_cli

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

