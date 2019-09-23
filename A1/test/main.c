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
        BTree t = newBTree(4);
        addKey(t, 1);
        addKey(t, 4);
        addKey(t, 7);
        addKey(t, 10);
        addKey(t, 17);
        addKey(t, 21);
        addKey(t, 31);
        addKey(t, 25);
        addKey(t, 19);
        addKey(t, 20);
       
        print_tree(t, t->root);
    }
    return 0;
}