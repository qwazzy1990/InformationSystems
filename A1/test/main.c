#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "utilities.h"
#include "stdbool.h"

bool DEBUG1 = true;
int main(int argc, char *argv[])
{
    char* s = NULL;
    if (DEBUG1)
    {
        BTree t = newBTree(3);
        addKey(t, 4);
        addKey(t, 7);
        addKey(t, 10);
        //addKey(t, 11);
       
        s = printTree(t);
        printf("%s\n", s);
        free(s);
    }
    return 0;
}