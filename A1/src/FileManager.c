
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


//read the original system file into a StringArray and sort the originl system file by key
StringArray readFromFile(char *fileName, int *fd)
{
    //open the file
    *fd = open(fileName, O_RDONLY, 0);
    if (*fd == -1)
    {
        printf("File could not be found\n");

        return NULL;
    }

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
//fileName will be called TreeData.txt
void writeToSortedFile(BTree t, StringArray a, char *fileName, int *fd)
{
    //open the file for writing. This file will bec
    *fd = open(fileName, O_CREAT | O_WRONLY);
    if (*fd == -1)
    {
        printf("File could not be found\n");

        return;
    }
    Record records[a->size];
    Block **blocks = calloc(a->size / 5 + 1000, sizeof(*blocks));
    forall(a->size / 5 + 1000)
    {
        blocks[x] = newBlock();
    }
    //write each string in one at a time
    forall(a->size)
    {
        setRecord(&records[x], a->strings[x]);
    }
    int numBlocks = 0;

    // Pack the records into blocks
    packRecords(blocks, records, a->size, &numBlocks);

    // write each block to the file
    forall(numBlocks)
    {
        writeBlock(t, *fd, blocks[x]);
    }
    /**TESTING PURPOSES**/

    forall(a->size / 5 + 1000)
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

void writeBlock(BTree t, int fd, Block *b)
{
    //write  the block number
    write(fd, &(b->blockNumber), sizeof(int));

    //for each record in the block
    forall(b->numRecords)
    {
        //the temp will be the key added to the tree t.
        //temp contains the key plus its block number.
        //will need to pare the keys in the tree to get its block number
        char *temp = calloc(strlen(b->records[x].key) + 10, 1);
        strcpy(temp, b->records[x].key);
        char xx[100];
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

    int offset = BLOCK_SIZE - (b->numRecords * (RECORD_SIZE) + sizeof(int));
    new_object(char *, s, offset);
    forall(offset)
    {
        s[x] = '|';
    }
    write(fd, s, offset);
    free(s);
}

//Writes a node to the tree file

void writeNode(char *fileName, BNode n, int idk)
{
    int fd = open(fileName, O_CREAT | O_WRONLY);
    if (fd == -1)
    {
        printf("File could not be found\n");

        return;
    }

    lseek(fd, 0, SEEK_SET);

    new_object(char *, s, 400);

    int num;

    //write the node's database address
    write(fd, &idk, sizeof(int));
    //write the number of keys
    write(fd, &(n->numKeys), sizeof(int));

    //write each key
    forall(n->numKeys)
    {
        char *temp = calloc(300, sizeof(char));
        strcpy(temp, n->keys[x]);
        write(fd, temp, KEY_SIZE);
        free(temp);
    }

    //if the leaf has been added then write 1
    if (n->beenAdded)
        num = 1;
    else
        num = 0; //else write 0
    write(fd, &num, sizeof(int));

    //if the leaf has been deleted write 1
    if (n->deleted)
        num = 1;
    else
        num = 0; //else write 0
    write(fd, &num, sizeof(int));

    close(fd);
}

BNode readNode(char *fileName, int idx)
{
    //open the file
    int fd = open(fileName, O_CREAT | O_RDONLY);
    if (fd == -1)
    {
        printf("File could not be found\n");

        return NULL;
    }
    lseek(fd, 0, SEEK_SET);

    //create a node
    BNode n = newBNode(8);
    //will be a leaf
    n->isLeaf = true;
    //will have no children
    n->numChilden = 0;
    //seek 4 from the index to get rid of its db address
    lseek(fd, 4, idx);
    int numKeys = 0;
    read(fd, &(numKeys), sizeof(int));
    forall(numKeys)
    {
        char *temp = calloc(100, sizeof(char));
        read(fd, temp, KEY_SIZE);
        add_key(n, temp);
        free(temp);
    }

    int num = 0;
    read(fd, &num, sizeof(int));
    printf("been added is %d\n", num);
    if (num == 0)
        n->beenAdded = false;
    else
        n->beenAdded = true;

    read(fd, &num, sizeof(int));
    printf("been deleted is %d\n", num);
    if (num == 0)
        n->deleted = false;
    else
        n->deleted = true;

    char *s = printNode(n);
    printf("%s\n", s);
    free(s);
    freeNode(n);

    close(fd);
    return NULL;
}
StringArray readBlock(int fd, int offset)
{
    if (fd == -1)
    {
        printf("File could not be found\n");

        return NULL;
    }
    lseek(fd, offset, SEEK_SET);
    char temp[10000];
    read(fd, temp, BLOCK_SIZE);
    String s = newString();
    addString(s, temp);
    String delims = newString();
    addString(delims, "|");
    StringArray sa = split(s, delims);
    lseek(fd, 0, SEEK_SET);
    freeString(s);
    freeString(delims);
    return sa;
}

char *findRecord(BTree t, char *key)
{
    if (t == NULL || key == NULL)
        return NULL;

    char *ret = calloc(1000, sizeof(char));
    //Go through all the leaves
    forall(t->numLeaves)
    {
        //get the current leaf
        BNode curNode = t->leaves[x];

        //go through all of its keys
        for (int i = 0; i < curNode->numKeys; i++)
        {

            //start at end of key and go backwards to find the data key without block num
            int idx = strlen(curNode->keys[i]) - 1;

            //since all keys are alpha non-numeric, any number will be the block number.
            //Therefore go backwards until you find a non-digit character
            while (isdigit(curNode->keys[i][idx]))
            {
                idx--;
            }
            idx++;

            //if key equals the curNode's key minus the block number then you found the key.
            if (strncmp(key, curNode->keys[i], idx) == 0)
            {

                int fd = open("TreeData.txt", O_RDONLY);
                StringArray a = readBlock(fd, returnBlockNumber(curNode->keys[i]) * BLOCK_SIZE + 4);
                if (a->size % 2 == 1)
                {
                    free(a->strings[a->size-1]);
                    a->size--;
                }
                strcat(ret, "BLOCK NUMBER: ");
                char *bn = calloc(10, sizeof(char));
                sprintf(bn, "%d", returnBlockNumber(curNode->keys[i]));
                strcat(ret, bn);
                strcat(ret, "\n");
                free(bn);

                //find the correct record
                for (int x = 0; x < a->size; x += 2)
                {

                    // if (strcmp(key, a->strings[x]) == 0)
                    // {
                    //     ret = calloc(VAL_SIZE, 1);
                    //     strcpy(ret, a->strings[x + 1]);
                    //     freeStringArray(a);
                    //     close(fd);
                    //     return ret;
                    // }

                    //for each record in the block
                    Record r;

                    //get the shit
                    char *shit = calloc(1000, sizeof(char));
                    strcat(shit, a->strings[x]);
                    strcat(shit, a->strings[x + 1]);

                    //set the record to the shity
                    setRecord(&r, shit);

                    //return the record as a string
                    char *temp = printRecord(&r);

                    //concatenate it to the block string called ret
                    strcat(ret, temp);

                    //free the shit
                    free(shit);
                    free(temp);

                    // printf("%s\n", a->strings[x]);
                }

                //free the fucking string array
                freeStringArray(a);
                close(fd);
                //return the ret
                return ret;
            }
        }
    }
    return ret;
}

char *rangeSearch(BTree t, char *k1, char *k2)
{
    if (t == NULL || k1 == NULL || k2 == NULL)
        return NULL;

    //char* s = findRecord(t, k1);
    //char* s2 = findRecord(t, k2);
    int bn1 = -1;
    int bn2 = -1;
    forall(t->numLeaves)
    {
        BNode n = t->leaves[x];
        for (int i = 0; i < n->numKeys; i++)
        {
            char *noNum = keyWithoutIndex(n->keys[i]);
            if (strcmp(noNum, k1) == 0)
            {
                bn1 = returnBlockNumber(n->keys[i]);
            }
            if (strcmp(noNum, k2) == 0)
            {
                bn2 = returnBlockNumber(n->keys[i]);
            }
            free(noNum);
        }
    }

    //FIX ME
    int fd = open("TreeData.txt", O_CREAT | O_RDONLY);
    lseek(fd, 0, SEEK_SET);
    if (bn1 == -1 || bn2 == -1 || fd == -1)
        return NULL;
    char *blocks = calloc(100 * BLOCK_SIZE, sizeof(char));
    foreach (bn1, bn2)
    {
        lseek(fd, x * BLOCK_SIZE, SEEK_SET);
        int blockNum;
        read(fd, &blockNum, 4);
        lseek(fd, 0, SEEK_SET);

        StringArray sa = readBlock(fd, x * BLOCK_SIZE + 4);
        char *block = printBlock(sa, blockNum);
        strcat(blocks, block);
        free(block);
    }
    printf("%s\n", blocks);
    return blocks;
}

char *keyWithoutIndex(char *key)
{
    char *temp = calloc(1000, 1);
    int idx = strlen(key) - 1;
    while (isdigit(key[idx]))
    {
        if (idx < 0)
            break;
        idx--;
    }
    idx++;
    strncpy(temp, key, idx);
    return temp;
}

int returnBlockNumber(char *key)
{

    int idx = 0;
    if (key == NULL)
    {
        printf("Null pointer exception\n");
        return -1;
    }
    //keep going through key until block number is found
    while (isdigit(key[idx]) == false)
    {
        idx++;
        if (idx > strlen(key))
            break;
    }

    //create a temp char*
    new_object(char *, s, 30);
    int count = 0;

    //go from start of block number till end
    foreach (idx, strlen(key))
    {
        s[count] = key[x];
        count++;
    }
    //return int version
    int bn = atoi(s);
    free(s);

    return bn;
}

void addRecord(BTree t, char *key, char *value, int *fd)
{
    *fd = open("TreeData.txt", O_RDONLY);
    if (*fd == -1)
    {
        printf("File could not be found\n");

        return;
    }
    lseek(*fd, -2 * BLOCK_SIZE, SEEK_END);

    //get the number of blocks
    int numBlocks = 0;
    read(*fd, &numBlocks, sizeof(int));
    lseek(*fd, 0, SEEK_SET);

    //for each block, check to see if the key goes at that block number
    printf("Num blocks is %d\n", numBlocks);
    forall(numBlocks + 1)
    {

        StringArray sa = readBlock(*fd, x * BLOCK_SIZE + 4);
        printf("Sa size is %d\n", sa->size);

        if (sa->size % 2 == 1)
        {
            strcpy(sa->strings[sa->size - 1], "");
            free(sa->strings[sa->size-1]);
            sa->size--;
        }

        if (x == 0 && strcmp(key, sa->strings[0]) < 0)
        {
            close(*fd);
            printf("Match found\n");
            *fd = open("TreeData.txt", O_WRONLY);

            // add the new key and value to the block
            addStringArray(sa, key);
            addStringArray(sa, value);
            Block *b = newBlock();
            b->blockNumber = x;
            b->numRecords = sa->size / 2;
            int count = 0;
            for (int i = 0; i < sa->size; i += 2)
            {
                char temp[1000];
                strcpy(temp, sa->strings[i]);
                strcat(temp, " ");
                strcat(temp, sa->strings[i + 1]);
                Record r;
                setRecord(&r, temp);
                r.blockNum = x;
                b->records[count] = r;
                count++;
            }
            freeStringArray(sa);

            lseek(*fd, x * BLOCK_SIZE, SEEK_SET);
            writeBlock(t, *fd, b);
            free(b);
            //write(*fd, temp, BLOCK_SIZE);
            close(*fd);
            break;
        }
        //printf("sa size is %d\n", sa->size);
        printf("key first key last key %s %s %s\n", key, sa->strings[0], sa->strings[sa->size - 2]);
        if (strcmp(sa->strings[0], key) <= 0 && (strcmp(sa->strings[sa->size - 2], key) >= 0))
        {
            close(*fd);
            printf("Match found\n");
            *fd = open("TreeData.txt", O_WRONLY);

            // add the new key and value to the block
            addStringArray(sa, key);
            addStringArray(sa, value);
            Block *b = newBlock();
            b->blockNumber = x;
            b->numRecords = sa->size / 2;
            int count = 0;
            for (int i = 0; i < sa->size; i += 2)
            {
                char temp[1000];
                strcpy(temp, sa->strings[i]);
                strcat(temp, " ");
                strcat(temp, sa->strings[i + 1]);
                Record r;
                setRecord(&r, temp);
                r.blockNum = x;
                b->records[count] = r;
                count++;
            }
            freeStringArray(sa);

            lseek(*fd, x * BLOCK_SIZE, SEEK_SET);
            writeBlock(t, *fd, b);
            free(b);
            //write(*fd, temp, BLOCK_SIZE);
            close(*fd);
            break;
        }

        if (x == numBlocks)
        {
            close(*fd);
            printf("Match found\n");
            *fd = open("TreeData.txt", O_WRONLY);

            // add the new key and value to the block
            addStringArray(sa, key);
            addStringArray(sa, value);
            Block *b = newBlock();
            b->blockNumber = x;
            b->numRecords = sa->size / 2;
            int count = 0;
            for (int i = 0; i < sa->size; i += 2)
            {
                char temp[1000];
                strcpy(temp, sa->strings[i]);
                strcat(temp, " ");
                strcat(temp, sa->strings[i + 1]);
                Record r;
                setRecord(&r, temp);
                r.blockNum = x;
                b->records[count] = r;
                count++;
            }
            freeStringArray(sa);

            lseek(*fd, x * BLOCK_SIZE, SEEK_SET);
            writeBlock(t, *fd, b);
            free(b);
            //write(*fd, temp, BLOCK_SIZE);
            close(*fd);
            break;
        }
    }
    close(*fd);
    return;
}

bool deleteRecord(BTree t, char *key)
{
    forall(t->numLeaves)
    {
        BNode n = t->leaves[x];
        if (n->deleted)
            continue;
        for (int k = 0; k < n->numKeys; k++)
        {

            int idx = strlen(n->keys[k]) - 1;
            while (isdigit(n->keys[k][idx]))
            {
                idx--;
            }
            if (strncmp(key, n->keys[k], idx) == 0)
            {
                printf("Deleting...\n%s %s\n", key, n->keys[k]);

                strcpy(n->keys[k], "");
                //n->keys--;
                if (n->keys == 0)
                {
                    n->deleted = true;
                }
                sortTree(t, t->root);

                return true;
            }
        }
    }

    return false;
}

//For Hash Map

void writeRecordHash(char *key, char *value, unsigned long hash)
{
    int fd = open("hashData.txt", O_CREAT | O_WRONLY);
    if (fd == -1)
    {
        printf("File could not be found\n");

        return;
    }
    lseek(fd, 0, SEEK_SET);
    lseek(fd, hash, SEEK_SET);
    Record r;
    char *temp = calloc(150, sizeof(char));
    strcpy(temp, key);
    strcat(temp, " ");
    strcat(temp, value);
    setRecord(&r, temp);
    writeRecord(fd, r);
    free(temp);
    close(fd);
}

void readRecordHash(unsigned long hash, Record *r)
{

    //open the file to read from
    int fd = open("hashData.txt", O_CREAT | O_RDONLY);
    if (fd == -1)
    {
        printf("File could not be found\n");

        return;
    }

    //create a temp
    char *temp = calloc(1000, sizeof(char));
    //seek to the hash value
    lseek(fd, hash, SEEK_SET);
    //read it in
    read(fd, temp, KEY_SIZE + VAL_SIZE);

    //split strings based on the '|' in the file
    String s = newString();
    addString(s, temp);
    String delims = newString();
    addString(delims, "|");
    StringArray sa = split(s, delims);
    free(temp);
    freeString(delims);
    freeString(s);

    printf("%s\n", sa->strings[0]);
    //copy the key in the file into  the record
    strcpy((*r).key, sa->strings[0]);
    //copy the value in the file to the record
    foreach (1, sa->size)
    {
        strcat((*r).value, sa->strings[x]);
    }
    printf("%s %s\n", (*r).key, (*r).value);
    //close the file
    close(fd);
}

void writeToHashFile(HashMap m, StringArray a, char *fileName)
{
    int fd = open(fileName, O_CREAT | O_WRONLY);
    if (fd == -1)
        return;
    lseek(fd, 0, SEEK_SET);
    forall(a->size)
    {
        Record r;
        setRecord(&r, a->strings[x]);
        put_hashmap(m, r.key, r.value);
        writeRecordHash(r.key, r.value, hash(m, r.key) * RECORD_SIZE);
    }

    close(fd);
}