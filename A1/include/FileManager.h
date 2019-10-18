#ifndef _FILE_MANAGER_
#define _FILE_MANAGER_
#include "StringArray.h"
#include "Record.h"
#include "BTree.h"
#include "HashMap.h"
#include <stdbool.h>


//For BPlus Tree
void writeToFile(char* fileName, int* fileNumber);


StringArray readFromFile(char* fileName, int* fileNumber);

void writeToSortedFile(BTree t, StringArray a, char* fileName, int* fd);

void writeRecord(int fd, Record r);
void writeBlock(BTree t, int fd, Block* b);

void writeNode(char* fileName, BNode n, int idx);

BNode readNode(char* fileName, int idx);

char* findRecord(BTree t, char* key);

char* rangeSearch(BTree t, char* k1, char* k2);

bool deleteRecord(BTree t, char* key);

int returnBlockNumber(char* key);

StringArray readBlock(int fd, int offset);

void addRecord(BTree t, char* key, char* value, int* fd);


//For HashMap

void writeRecordHash(char* key, char* value, unsigned long hash);
void readRecordHash(unsigned long hash, Record* r);
void writeToHashFile(HashMap m, StringArray a, char* fileName);
#endif