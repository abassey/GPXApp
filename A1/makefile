CC = gcc
CFLAGS = -Wall -std=c11 -pedantic -g
LFLAGS = -c -fpic

all: list parser

parser: ./src/GPXParser.c ./src/XMLParser.c ./include/GPXParser.h
	$(CC) $(CFLAGS) $(LFLAGS) -Iinclude ./src/GPXParser.c -o ./bin/gpxparser.o
	$(CC) $(CFLAGS) $(LFLAGS) -Iinclude ./src/XMLParser.c -o ./bin/xmlparser.o
	$(CC) $(CFLAGS)  -shared ./bin/gpxparser.o ./bin/xmlparser.o -o ./bin/libgpx.so

list: ./src/LinkedListAPI.c ./include/LinkedListAPI.h
	$(CC) $(CFLAGS) $(LFLAGS) -Iinclude ./src/LinkedListAPI.c -o ./bin/list.o
	$(CC) $(CFLAGS)  -shared ./bin/list.o -o ./bin/liblist.so

clean:
	rm -rf ./bin/*.o
	rm -rf ./bin/*.so
