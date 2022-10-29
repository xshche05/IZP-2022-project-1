CC=gcc

CFLAGS=-std=c99 -Wall -Wextra -Werror -c

all: t9search clean

main.o: t9search.c
	$(CC) $(CFLAGS) main.c

t9search: main.o
	$(CC) main.o -o t9search

clean:
	rm -rf *.o