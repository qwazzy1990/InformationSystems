#ifndef _FILE_MANAGER_
#define _FILE_MANAGER_
#include "StringArray.h"
#include "Record.h"
#include "BTree.h"
#include <stdbool.h>



void writeToFile(char* fileName, int* fileNumber);


StringArray readFromFile(char* fileName, int* fileNumber);

void writeToSortedFile(BTree t, StringArray a, char* fileName, int* fd);

void writeRecord(int fd, Record r);
void writeBlock(BTree t, int fd, Block* b);

char* findRecord(BTree t, char* key);

bool deleteRecord(BTree t, char* key);

int returnBlockNumber(char* key);

StringArray readBlock(int fd, int offset);

void addRecord(BTree t, char* key, char* value, int* fd);
#endif