CC = gcc
CFLAGS = -g -O0

all: shell test

shell: shell.c
	$(CC) -o shell shell.c $(CFLAGS)

test: test.c
	$(CC) -o test test.c $(CFLAGS)

.PHONY: clean

clean:
	rm shell
