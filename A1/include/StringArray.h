/**
 * NAME: Patrick Di Salvo
 * 0629465
 * */


#ifndef _STRING_ARRAY_
#define _STRING_ARRAY_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "DynamicString.h"
#include "utilities.h"




//#define destroy_string_array(a) destroy_string_array_real(&a)
struct stringarray
{
    char** strings;
    int size;
};typedef struct stringarray* StringArray;

StringArray newStringArray( void );

StringArray split(String s, String delims);


void freeStringArray(StringArray a);

void addStringArray(StringArray a, char* val);



#endif