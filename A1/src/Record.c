

#include "Record.h"
#include "DynamicString.h"
#include "StringArray.h"
#include "utilities.h"
#include "BTree.h"



void setRecord(Record *r, char *s)
{
    int i = 0;
    int count = 0;
    while (s[i] != ' ')
    {
        r->key[count] = s[i];
        i++;
        count++;
        if(i > strlen(s))break;
    }
    r->key[count] = '\0';
    count = 0;
    i++;
    while (s[i] != '\0')
    {
        r->value[count] = s[i];
        i++;
        count++;
    }
    r->value[count] = '\0';
}

Block *newBlock(void)
{
    new_object(Block *, b, 1);
    b->blockNumber = -1;
    b->numRecords = 0;
    return b;
}

void packRecords(Block **b, Record *records, int numRecords, int* numBlocks)
{
    int blockNum = 0;
    for (int i = 0; i < numRecords; i++)
    {

        b[blockNum]->records[b[blockNum]->numRecords] = records[i];
        b[blockNum]->records[b[blockNum]->numRecords].blockNum = blockNum;
        b[blockNum]->numRecords++;
        if (b[blockNum]->numRecords >= NUM_RECORDS)
        {
            b[blockNum]->blockNumber = blockNum;
            blockNum++;
            
        }
    }
    *numBlocks = blockNum;
}


char* printRecord(void * r)
{
    Record* rr = (Record*)r;
    char* temp = calloc(300, sizeof(char));
    strcat(temp, "RECORD KEY: ");
    strcat(temp, rr->key);
    strcat(temp, "\n");
    strcat(temp, "RECORD VALUES: ");
    strcat(temp, rr->value);
    strcat(temp, "\n");
    return temp;
}

char* printBlock(StringArray sa, int bn)
{
    if(sa->size %2 == 1) sa->size--;

    char* tempNum = calloc(100, sizeof(char));
    sprintf(tempNum, "%d", bn);
    char* ss = calloc(BLOCK_SIZE+300, sizeof(char));
    strcat(ss, "BLOCK NUMBER: ");
    strcat(ss, tempNum);
    strcat(ss, "\n");
    free(tempNum);
    for(int x = 0; x < sa->size; x+=2)
    {
        Record r;
        char* shit = calloc(1000, sizeof(char));
        strcat(shit, sa->strings[x]);
        strcat(shit, " ");
        strcat(shit, sa->strings[x+1]);
        setRecord(&r, shit);
        char* tempRec = printRecord(&r);
            strcat(ss, tempRec);
        free(tempRec);
    }

    return ss;
}
