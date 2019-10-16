#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "StringArray.h"
#include "DynamicString.h"

StringArray newStringArray( void )
{
    new_object(StringArray, s, 1);
    s->strings = NULL;
    s->size = 0;
    return s;
}
StringArray split(String s, String delims)
{
    StringArray a = newStringArray();
    a->strings = calloc(1, sizeof(char*));
    
    char *token = strtok(s->string, delims->string);
    int x = 0;
    int memSize = 2;
    while(token != NULL)
    {
        a->strings[x] = calloc(strlen(token)+1, sizeof(char));
        strcpy(a->strings[x], token);
        a->strings = realloc(a->strings, sizeof(char*)*memSize);
        memSize++;
        a->size++;
        x++;
        token = strtok(NULL, delims->string);
    }
    return a;
}

void freeStringArray(StringArray a)
{
    if(a == NULL)return;
    if(a->strings == NULL)
    {
        free(a);
    }
    forall(a->size)
    {
        free(a->strings[x]);
    }
    free(a->strings);
    free(a);
}


void addStringArray(StringArray a, char* val)
{
    if(a == NULL || val == NULL)return;
    a->size++;
    a->strings = realloc(a->strings, a->size*sizeof(char*));
    a->strings[a->size-1] = calloc(strlen(val)+1, sizeof(char));
    strcpy(a->strings[a->size-1], val);
}