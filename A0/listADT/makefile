CC = gcc
CFLAGS = -Wall -std=c11 -pedantic -g
LFLAGS = -c -fpic

all: list

list: ./src/LinkedListAPI.c ./include/LinkedListAPI.h
	$(CC) $(CFLAGS) $(LFLAGS) -Iinclude ./src/LinkedListAPI.c -o ./bin/list.o
	$(CC) $(CFLAGS)  -shared ./bin/list.o -o ./bin/liblist.so

clean:
	rm -rf ./bin/*.o
	rm -rf ./bin/*.so
