#ifndef _RECORD_
#define _RECORD_
#include <string.h>

#define KEY_SIZE 30
#define VAL_SIZE 260
struct record{
    char key[KEY_SIZE];
    char value[VAL_SIZE];
};
typedef struct record Record;

//Set a records key and value from s
void setRecord(Record* r, char* s);
#endif
