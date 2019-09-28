#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "utilities.h"
#include "stdbool.h"
#include "math.h"
#include "DynamicString.h"
#include "StringArray.h"
#include "Record.h"

bool DEBUG1 = false;
bool DEBUG2 = true;
int main(int argc, char *argv[])
{
    char *s = NULL;
    if (DEBUG1)
    {
        BTree t = newBTree(4);
        //printf("Min keys is, Min Children is %f %f\n", ceil(t->m/2)-1, ceil(t->m/2));
         addKey(t, "a:1");  //1
        addKey(t, "b:4");  //4
        addKey(t, "c:7");  //7
         addKey(t, "d:10"); //10
         addKey(t, "e:17");
           addKey(t, "i:21");
         addKey(t, "m:31");
         addKey(t, "j:25");
         addKey(t, "g:19");
        addKey(t, "h:20");
       
        // addKey(t, "z:42");
        print_tree(t, t->root);
    }
    if (DEBUG2)
    {

        String theFile = readFile(argv[1]);

        String delims = newString();
        addString(delims, "\r\n");
        StringArray a = split(theFile, delims);
        freeString(theFile);
        freeString(delims);

        qsort(a->strings, a->size, sizeof(char *), stringCompare);
        BTree t = newBTree(8);
        forall(a->size)
        {
            Record r;
            setRecord(&r, a->strings[x]);
            addKey(t, r.key);
        }
        freeStringArray(a);
        print_tree(t, t->root);
        deleteTree(t);
        // Record r;
        // setRecord(&r, a->strings[1]);
        // printf("%s|%s\n", r.key, r.value);
        // freeString(theFile);
        // freeString(delims);
        // freeStringArray(a);
        //String delims = newString();
    }
    return 0;
}