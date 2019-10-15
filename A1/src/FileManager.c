
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
#include <ctype.h>

//open the original system file from argv[1] and write to it
void writeToFile(char *fileName, int *fd)
{
    String theFile = readFile(fileName);
    *fd = open("data.txt", O_CREAT | O_WRONLY, 0);
    write(*fd, theFile->string, strlen(theFile->string));
    close(*fd);
    freeString(theFile);
}

//read the original system file into a StringArray and sort the originl system file by key
StringArray readFromFile(char *fileName, int *fd)
{
    //open the file
    *fd = open(fileName, O_RDONLY, 0);

    //used to calculate the numbe of bytes in the file
    int len = 0;
    char temp[5];
    while (read(*fd, temp, 1))
    {
        len++;
        lseek(*fd, len, SEEK_SET);

    } //done getting numbe of bytes

    ///create a char* s = to number of bytes in the file
    char *s = calloc(len + 1, 1);
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
void writeToSortedFile(BTree t, StringArray a, char *fileName, int* fd)
{
    //open the file for writing. This file will bec
    *fd = open(fileName, O_CREAT | O_WRONLY);


    Record records[a->size];
    Block **blocks = calloc(a->size / 5 + 100, sizeof(*blocks));
    forall(a->size / 5 + 100)
    {
        blocks[x] = newBlock();
    }
    //write each string in one at a time
    forall(a->size)
    {
        setRecord(&records[x], a->strings[x]);
    }
    int numBlocks = 0;

    //Pack the records into blocks
    packRecords(blocks, records, a->size, &numBlocks);

   
    //write each block to the file
    forall(numBlocks)
    {
        writeBlock(t, *fd, blocks[x]);
    }
    /**TESTING PURPOSES**/
    
    forall(a->size / 5 + 100)
    {
        free(blocks[x]);
    }


    free(blocks);
    freeStringArray(a);
    close(*fd);
}

void writeRecord(int fd, Record r)
{
    //create a temp for difference of max size of key - actual key len
    char *temp = NULL;

    //write the key
    write(fd, r.key, strlen(r.key));

    //set the temp to the difference between max key and actual key
    temp = calloc(KEY_SIZE - strlen(r.key), 1);
    forall(KEY_SIZE - strlen(r.key))
        temp[x] = '|';
    //write it to the file
    write(fd, temp, (KEY_SIZE)-strlen(r.key));
    free(temp);
    temp = NULL;

    //do the same for the value as for the key
    write(fd, r.value, strlen(r.value));
    temp = calloc(VAL_SIZE - strlen(r.value), 1);

    forall(VAL_SIZE - strlen(r.value))
        temp[x] = '|';

    write(fd, temp, VAL_SIZE - strlen(r.value));
    free(temp);
}

void writeBlock(BTree t, int fd, Block* b)
{
    //write  the block number
    write(fd, &(b->blockNumber) ,sizeof(int));

    //for each record in the block
    forall(b->numRecords)
    {
        //the temp will be the key added to the tree t.
        //temp contains the key plus its block number.
        //will need to pare the keys in the tree to get its block number
        char* temp = calloc(strlen(b->records[x].key)+10, 1);
        strcpy(temp, b->records[x].key);
        char xx[10];
        //get the block number
        sprintf(xx, "%d", b->records[x].blockNum);
        //concatenate block number to the key in the file
        strcat(temp, xx);

        //add the file key plus block number as the key in the BTree
        addKey(t, temp);
        //write the record to the file descriptor
        writeRecord(fd, b->records[x]);
        
        free(temp);
    }

    int offset = 1024 - (b->numRecords*(KEY_SIZE+VAL_SIZE)+sizeof(int));
    new_object(char*, s, offset);
    forall(offset)
    {
        s[x] = '|';
    }
    write(fd, s, offset);
    free(s);
  
}


char* findRecord(BTree t, char* key)
{
    if(t == NULL || key == NULL)return NULL;

    //Go through all the leaves
    forall(t->numLeaves)
    {
        //get the current leaf
        BNode curNode = t->leaves[x];
        
        //go through all of its keys
        for(int i = 0; i < curNode->numKeys; i++)
        {
            
            //start at end of key and go backwards to find the data key without block num
            int idx = strlen(curNode->keys[i])-1;
           
            //since all keys are alpha non-numeric, any number will be the block number.
            //Therefore go backwards until you find a non-digit character
            while(isdigit(curNode->keys[i][idx]))
            {
                idx--;
            }
            idx++;
            
            //if key equals the curNode's key minus the block number then you found the key.
            if(strncmp(key, curNode->keys[i], idx)==0)
            {
                
                printf("%d\n", returnBlockNumber(curNode->keys[i]));
                char temp[10000];
                int fd = open("sortedData.txt", O_RDONLY);
                lseek(fd, returnBlockNumber(curNode->keys[i])*1024 + 4, SEEK_SET);
                read(fd, temp, 1024);
                printf("%s\n", temp);
                free(temp);
            }
            
            
            

        }
    }
    return NULL;
}


int returnBlockNumber(char* key)
{

    int idx = 0;
    //keep going through key until block number is found
    while(isdigit(key[idx])==false)
    {
        idx++;
    }

    //create a temp char*
    new_object(char*, s, 30);
    int count = 0;

    //go from start of block number till end
    foreach(idx, strlen(key))
    {
        s[count] = key[x];
        count++;
    }
    //return int version
    int bn = atoi(s);
    free(s);

    return bn;
}