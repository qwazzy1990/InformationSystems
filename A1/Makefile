CC = gcc
CFLAGS = -std=c11 -Wall -g -pedantic
INPUT=testTwo.txt


all: clean

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

tree: makea1 runa1

makea1: BIN INCLUDE SRC OBJ TEST LIB
	$(CC) $(CFLAGS) -lm -Iinclude test/TreeMain.c src/*.c -o ./bin/a1

runa1: makea1
	./bin/a1 $(INPUT)

hash: makeh1 runh1

makeh1: BIN INCLUDE SRC OBJ TEST LIB
	$(CC) $(CFLAGS) -lm -Iinclude test/HashMain.c src/*.c -o ./bin/a1hash

runh1: makeh1
	./bin/a1hash $(INPUT)


clean: 
	if [ -d "bin" ]; then rm -rf ./bin/; fi;


cleanFiles:
	if test -f TreeData.txt; then rm TreeData.txt; fi;
	if test -f hashData.txt; then rm hashData.txt; fi;


push:
	git add -A 
	git commit -m "$(m)"
	git push
