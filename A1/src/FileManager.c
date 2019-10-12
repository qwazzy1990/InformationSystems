
#include "FileManager.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "utilities.h"
#include "stdbool.h"
#include "math.h"
#include "DynamicString.h"
#include "StringArray.h"
#include "Record.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


//open the original system file from argv[1] and write to it
void writeToFile(char* fileName, int* fd)
{
    String theFile = readFile(fileName);
    *fd = open("data.txt", O_CREAT | O_WRONLY, 0);
    write(*fd, theFile->string, strlen(theFile->string));
    close(*fd);
    freeString(theFile);
}


//read the original system file into a StringArray and sort the originl system file by key
StringArray readFromFile(char* fileName, int* fd)
{
    //open the file
    *fd = open(fileName, O_RDONLY, 0);

    //used to calculate the numbe of bytes in the file
    int len = 0;
    char temp[5];
    while(read(*fd, temp, 1))
    {
         len++;
         lseek(*fd, len, SEEK_SET);

    }//done getting numbe of bytes

    ///create a char* s = to number of bytes in the file
    char* s = calloc(len+1, 1);
    //go back to beginning of the file
    lseek(*fd, 0, SEEK_SET);
    //read in the data from the file
    read(*fd, s, len);

    //create a string object and add char*
    String ss = newString();
    addString(ss, s);

    //free char*
    free(s);

    //create a string of delims
    String delims = newString();
    addString(delims, "\r\n");

    //create the stringarray
    StringArray a = split(ss, delims);

    //sort the string array. Now the original data is sorted by key
    qsort(a->strings, a->size, sizeof(char *), stringCompare);

    //free memory
    freeString(ss);
    freeString(delims);
    

    close(*fd);
    return a;
}


//Write the sorted string array to the file.
//fileName will be called sortedData.txt
void writeToSortedFile(StringArray a, char* fileName)
{
    //open the file for writing. This file will bec
    int fd = open(fileName, O_CREAT | O_WRONLY);

    Record records[a->size];
    Block** blocks = calloc(a->size/5 + 100, sizeof(*blocks));
    forall(a->size/5 + 100)
    {
        blocks[x] = newBlock();
    }
    //write each string in one at a time
    forall(a->size)
    {
        setRecord(&records[x], a->strings[x]);
    }
    int numBlocks = 0;
    packRecords(blocks, records, a->size, &numBlocks);
    forall(numBlocks)
    {
        Block* block = blocks[x];
        printf("Block Number %d Number of records %d\n", block->blockNumber, block->numRecords);
    }

    //close the file
    forall(a->size/5 + 100)
    {
        free(blocks[x]);
    }
    
    write(fd, &numBlocks, sizeof(int));
    printf("testing read back\n");
    int tempTwo;

    close(fd);
    fd = open(fileName, O_RDONLY);
    read(fd, &tempTwo, sizeof(int));
    printf("tempTwo is %d\n", tempTwo);
    free(blocks);
    freeStringArray(a);
    close(fd);
}
