CC=gcc

CFLAGS=-Wall -Wextra -Werror

all: t9search

t9search: main.o
	$(CC) $(CFLAGS) main.o -o t9search

main.o: main.c
	$(CC) $(CFLAGS) main.c