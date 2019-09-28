

#include "Record.h"
#include "DynamicString.h"
#include "StringArray.h"
#include "utilities.h"

void setRecord(Record* r, char* s)
{
    int i = 0;
    int count = 0;
    while(s[i] != ' ')
    {
        r->key[count] = s[i];
        i++;
        count++;
    }
    r->key[count] = '\0';
    count = 0;
    i++;
    while(s[i] != '\0')
    {
        r->value[count] = s[i];
        i++;
        count++;
    }
    r->value[count] = '\0';


}