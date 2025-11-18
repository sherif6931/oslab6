# This file was auto-generated.

CC := gcc
CFLAGS := -Wall -Wextra -Iincl -DSIGFLAG

BIN := bin/out

SRCS := src/main.c src/rint.c
OBJS := $(SRCS:.c=.o)

all: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $(BIN)

src/%.o: src/%.c incl/_rint.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(BIN)

.PHONY: all clean

