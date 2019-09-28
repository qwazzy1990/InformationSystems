#ifndef _BLOCK_
#define _BLOCK_


#include "Record.h"
#define RECORD_SIZE 830
#define BLOCK_SIZE 1024

struct block
{
    Record records[34];

    int numRecords;

};typedef struct block Block;

//Add a record to a block
void addRecord(Block b, Record r);



#endif