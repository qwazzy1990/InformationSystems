#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "utilities.h"
#include "stdbool.h"

bool DEBUG1 = true;
int main(int argc, char *argv[])
{

    if (DEBUG1)
    {
        BTree t = newBTree(3);
        addKey(t, 4);
        addKey(t, 7);
        addKey(t, 11);
        forall(t->numLeaves)
        {
            BNode leaf = t->leaves[x];
            for (int i = 0; i < leaf->numKeys; i++)
            {
                printf("%d\n", leaf->keys[i]);
            }
        }
    }
    return 0;
}