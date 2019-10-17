#ifndef _RECORD_
#define _RECORD_
#include <string.h>

#define KEY_SIZE 30
#define VAL_SIZE 134
#define BLOCK_SIZE 1024
#define NUM_RECORDS 5
#define RECORD_SIZE 164



struct record{
    char key[KEY_SIZE];
    char value[VAL_SIZE];
    int blockNum;
};
typedef struct record Record;


struct block{

    Record records[20];
    int numRecords;
    int blockNumber;
};typedef struct block Block;

//Set a records key and value from s
void setRecord(Record* r, char* s);

Block* newBlock( void );
void packRecords(Block** b, Record* r, int numRecords, int* numBlocks);


#endif
