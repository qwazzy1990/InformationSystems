#ifndef _STRING_
#define _STRING_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include "utilities.h"

struct string{
    char* string;
    int len;
};typedef struct string* String;

String newString(void);

void addString(String s, char* val);

void freeString(String s);
int stringCompare(const void* a, const void * b);
String readFile(char *fileName);




#endif
