CC = gcc
CFLAGS = -Wall -std=c11 -pedantic -g
LFLAGS = -c -fpic

all: parser

parser: ./src/GPXParser.c ./src/helper.c ./include/helper.h ./include/GPXParser.h ./include/LinkedListAPI.h ./src/LinkedListAPI.c
	$(CC) $(CFLAGS) $(LFLAGS) -l -Iinclude -I/usr/include/libxml2  `xml2-config --cflags --libs` ./src/GPXParser.c -o ./bin/gpxparser.o
	$(CC) $(CFLAGS) $(LFLAGS) -Iinclude ./src/LinkedListAPI.c -o ./bin/list.o
	$(CC) $(CFLAGS) $(LFLAGS) -l -Iinclude -I/usr/include/libxml2 ./src/helper.c -o ./bin/helper.o
	$(CC) $(CFLAGS) -shared ./bin/gpxparser.o ./bin/helper.o ./bin/list.o -o ./bin/libgpx.so

clean:
	rm -rf ./bin/*.o
	rm -rf ./bin/*.so
