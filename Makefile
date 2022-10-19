CC=gcc

CFLAGS=-c -Wall -Wextra -Werror

all: t9search clean

main.o: main.c
	$(CC) $(CFLAGS) main.c

t9search: main.o
	$(CC) main.o -o t9search

clean:
	rm -rf *.o