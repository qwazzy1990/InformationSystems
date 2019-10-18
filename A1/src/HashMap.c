
#include <stdio.h>
#include <stdlib.h>
#include "HashMap.h"
#include <assert.h>

/*****ENCAPSULATED FUNCTIONS AND VARIABLES NOT DIRECTLY 
                                                   AVAILABLE TO API USERS*********/

unsigned long hash(HashMap m, char *str)
{
    int i = 0;
    unsigned long hash = 5381;
    int c;

    while ((c = str[i]) != 0)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        i++;
    }

    hash = hash % m->tableSize;
    return hash;
}

static MapNode new_empty_map(void)
{
    MapNode newNode = (MapNode)calloc(1, sizeof(*newNode));
    newNode->data = NULL;
    newNode->key = NULL;
    newNode->inUse = 0;
    return newNode;
}

static ErrorCode destroy_mapnode_real(MapNode *n)
{
    if (n == NULL)
    {
        return DNE;
    }
    if (*n == NULL)
    {
        return DNE;
    }

    free((*n)->key);
    free(*n);
    *n = NULL;
    return OK;
}

unsigned long get_data_index(HashMap map, char *key)
{
    assert(map != NULL);
    assert(map->table != NULL);
    unsigned long index = hash(map, key);
    assert(index < map->tableSize);
    unsigned long start = index;
    if (map->table[index]->key == NULL)
        return -1;
    if (strncmp(key, map->table[index]->key, strlen(key + 1)) == 0)
        return index;

    assert(map->table[index] != NULL);
    assert(map->table[index]->key != NULL);
    assert(map->table[index]->data != NULL);

    //while
    while (strncmp(key, map->table[index]->key, strlen(key) + 1) != 0)
    {
        index++;
        //if
        if (index == (map->tableSize))
        {
            index = 0;
        } //enidif

        //if
        if (index == start)
        {
            return -1;
        } //endif
        MapNode temp = map->table[index];
        if (temp->inUse == 1)
        {
            if (strncmp(key, temp->key, strlen(key + 1)) == 0)
                return index;
        }
        else
        {
            continue;
        }

    } //endwhile
    return index;
}

static unsigned long table_size[] = {

    7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
    16381, 32749, 65521, 131071,

    262143, 524287, 1048575, 2097151, 4194303, 8388607,
    16777211, 33554431, 67108863, 134217727, 268435455,
    536870911, 1073741823, 2147483647};

/*****
                         * Following Code is Open source API code for HashMap.h
                         * ***Comments on the functionality of the functions can be found 
                         * ***in HashMap.h
                         * ********/

/***CONSTRUCTORS*****/

HashMap new_hashmap(PrintFunc p, DeleteFunc d, unsigned int sizeOfTable)
{
    int max = sizeof(table_size) / sizeof(table_size[0]);
    int index = 0;
    assert(sizeOfTable >= 1);
    if (p == NULL)
        return NULL;
    if (d == NULL)
        return NULL;
    HashMap map = (HashMap)calloc(1, sizeof(Map));
    assert(map != NULL);
    if (sizeOfTable >= max)
    {
        index = max - 1;
        map->table = (MapNodes)calloc(table_size[index], sizeof(MapNode));
    }
    else
    {
        index = sizeOfTable - 1;
        map->table = (MapNodes)calloc(table_size[index], sizeof(MapNode));
    }
    assert(map->table != NULL);
    for (int i = 0; i < table_size[index]; i++)
    {
        map->table[i] = new_empty_map();
    }
    map->size = 0;
    map->tableSize = table_size[index];
    map->printData = p;
    map->deleteData = d;
    return map;
}

MapNode new_mapnode(char *k, AnyData d)
{
    MapNode newNode = (MapNode)calloc(1, sizeof(*newNode));
    newNode->key = calloc(strlen(k) + 1, sizeof(char));
    strcpy(newNode->key, k);
    newNode->data = d;
    newNode->inUse = 1;
    return newNode;
}

/****ACCESSORS****/

ErrorCode get_hashmap(HashMap map, char *key, AnyData *value)
{
    assert(map != NULL);
    assert(value != NULL);
    ErrorCode status;
    unsigned long index = get_data_index(map, key);
    if (index == -1)
    {
        status = DNF;
        return status;
    }
    *value = map->table[index]->data;
    if (*value == NULL)
        return DNE;
    AnyData t = *value;
    char *s = map->printData(t);
    free(s);
    status = OK;
    return status;
}

unsigned long current_size_hashmap(HashMap map)
{
    if (map == NULL)
        return -1;
    unsigned long size = map->size;
    return size;
}

unsigned long table_size_hashmap(HashMap map)
{
    if (map == NULL)
        return -1;
    unsigned long tableSize = map->tableSize;
    return tableSize;
}

/*********SETTERS***********/

ErrorCode put_hashmap(HashMap map, char *key, AnyData value)
{
    assert(map != NULL);
    if (value == NULL)
        return DNE;
    if (map->size >= map->tableSize)
        return FULL;
    unsigned long index = hash(map, key);
    index = index % map->tableSize;

    assert(index < map->tableSize);
    ErrorCode status;
    status = iterate_hashmap(map, &index);
    if (status == OOM)
    {
        return status;
    }
    MapNode temp = (MapNode)map->table[index];
    if (temp->inUse)
    {
        while (map->table[index]->inUse)
            index++;

        MapNode newNode = new_mapnode(key, value);
        destroy_mapnode(map->table[index]);
        map->table[index] = newNode;
    }
    else
    {
        destroy_mapnode(map->table[index]);
        MapNode newNode = new_mapnode(key, value);
        map->table[index] = newNode;
    }

    map->size++;
    return status;
}

/************DESTROYERS************/
ErrorCode remove_from_hashmap(HashMap map, char *key)
{
    assert(map != NULL);
    unsigned long index = get_data_index(map, key);
    if (index == -1)
        return DNE;
    map->deleteData(map->table[index]->data);
    map->table[index]->data = NULL;
    map->table[index]->inUse = 0;
    free(map->table[index]->key);
    return OK;
}

ErrorCode destroy_hashmap_real(HashMap *map)
{

    ErrorCode code;
    HashMap m = *map;
    for (int i = 0; i < (m)->tableSize; i++)
    {
        MapNode temp = (m)->table[i];
        if (temp->inUse == 0)
        {
            destroy_mapnode((*map)->table[i]);
        }
        else
        {
            m->deleteData((*map)->table[i]->data);
            destroy_mapnode((*map)->table[i]);
        }
    }
    (*map)->tableSize = 0;
    (*map)->size = 0;
    free((*map)->table);
    (*map)->table = NULL;
    free(*map);
    *map = NULL;
    code = OK;
    return code;
}

/***************ITERATORS**************/

ErrorCode iterate_hashmap(HashMap map, unsigned long *start)
{
    Iterator itr;
    unsigned long index = *start;
    itr.current = (MapNode)map->table[index];
    MapNode temp = (MapNode)itr.current;
    unsigned long beginning = index;
    while (temp->inUse != 0)
    {
        (index)++;
        if (index >= map->tableSize)
        {
            index = 0;
        }
        if (index == beginning)
            return OOM;
        itr.current = (MapNode)map->table[index];
        temp = (MapNode)itr.current;
    }
    return OK;
}

/******************PRINTERS*************/
char *map_to_string(HashMap map)
{
    assert(map != NULL);
    assert(map->table != NULL);
    Iterator itr;
    itr.current = NULL;
    char *printer = calloc(100, sizeof(char));
    int memSize = 100;
    for (int i = 0; i < map->tableSize; i++)
    {
        itr.current = map->table[i];
        MapNode n = (MapNode)itr.current;
        if (n->inUse == 1)
        {
            new_object(char *, k, 100);
            strcpy(k, "KEY: VALUE: ");
            strcat(printer, k);
            memSize++;
            printer = realloc(printer, memSize * sizeof(char));
            char *temp = map->printData(n->data);
            memSize += strlen(temp);
            printer = realloc(printer, memSize * sizeof(char));
            strcat(printer, temp);
            strcat(printer, "\n\n");
            free(temp);
            free(k);
        }
    }
    return printer;
}

void print_error_code(ErrorCode code)
{
    if (code == OK)
        printf("Status OK\n");
    if (code == OOM)
        printf("Status Out of Memory\n");
    if (code == DNF)
        printf("Status Data not found\n");
    if (code == DNE)
        printf("Status Data does not exist\n");
    if (code == FULL)
        printf("Status Table Full\n");
}



ErrorCode containsHash(HashMap m, char* key)
{
    unsigned long index = hash(m, key);

    if(m->table[index]->inUse == false)return false;

    while(m->table[index]->inUse)
    {
        if(strcmp(m->table[index]->key, key)==0)return true;
        index++;
    }
    return false;
}