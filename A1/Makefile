CC = gcc
CFLAGS = -std=c11 -Wall -g -pedantic

all: a1

BIN:
	if [ ! -d "bin" ]; then mkdir bin; fi;

INCLUDE:
	if [ ! -d "include" ]; then mkdir include; fi;

SRC: 
	if [ ! -d "src" ]; then mkdir src; fi;

OBJ:
	if [ ! -d "obj" ]; then mkdir obj; fi;

TEST:
	if [ ! -d "test" ]; then mkdir test; fi;

LIB:
	if [ ! -d "lib" ]; then mkdir lib; fi;

a1: makea1 runa1

makea1: BIN INCLUDE SRC OBJ TEST LIB
	$(CC) $(CFLAGS) -lm -Iinclude test/*.c src/*.c -o ./bin/a1

runa1: makea1
	./bin/a1 test.txt

clean: 
	if [ -d "bin" ]; then rm -rf ./bin/; fi;