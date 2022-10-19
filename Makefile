CC=gcc

CFLAGS=-Wall -Wextra -Werror

all: t9search

main.o: main.c
	$(CC) $(CFLAGS) main.c

t9search: main.o
	$(CC) main.o -o t9search