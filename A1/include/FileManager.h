#ifndef _FILE_MANAGER_
#define _FILE_MANAGER_
#include "StringArray.h"


void writeToFile(char* fileName, int* fileNumber);


StringArray readFromFile(char* fileName, int* fileNumber);

void writeToSortedFile(StringArray a, char* fileName);

#endif