#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "DynamicString.h"
#include <ctype.h>

String newString(void)
{
    new_object(String, s, 1);
    s->len = 0;
    return s;
}

void addString(String s, char* val)
{
    if(val == NULL)return;
    if(s->string == NULL)
        s->string = calloc(strlen(val)+1, sizeof(char));

    strcpy(s->string, val);
    s->len = strlen(val);
}

void freeString(String s)
{
    if(s == NULL)return;
    if(s->string != NULL)
        free(s->string);
    free(s);
}

int stringCompare(const void* a, const void * b)
{
    if(a == NULL || b == NULL)return 0;

    char** s1 = (char**)a;
    char** s2 = (char**)b;
    return strcmp(*s1, *s2);
}

String readFile(char *fileName)
{
    String theFile = newString();
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return theFile;
    }
    rewind(fp);
    theFile->string = (char *)calloc(2, sizeof(char));

    int mem_size = 2;
    int count = 0;
    char c;
    while (!feof(fp))
    {
        c = fgetc(fp);
        if (feof(fp) != 0)
            break;

        theFile->string[count] = c;
        theFile->string[count + 1] = '\0';
        count++;
        mem_size++;
        theFile->string = (char *)realloc(theFile->string, sizeof(char) * mem_size);
        if (theFile->string == NULL)
        {
            return NULL;
        }
    }
    theFile->string[count] = '\0';
    theFile->len = strlen(theFile->string);
    return theFile;

    return NULL;
}

void toLower(char* s)
{
    if(s == NULL)return;
    forall(strlen(s))
    {
        s[x] = tolower(s[x]);
    }
}