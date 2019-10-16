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
#include "FileManager.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool DEBUG1 = false;
bool DEBUG2 = true;
bool DEBUG3 = false;
bool DEBUG4 = false;
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
        printf("size is %d\n", a->size);
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
        printf("Number of leaves %d\n", t->numLeaves);
        deleteTree(t);
    }

    if (DEBUG3)
    {
        Record records[10];
        forall(10)
        {
            strcpy(records[x].key, "k1");
            strcpy(records[x].value, "val");
        }

        Block **blocks = (Block **)calloc(2, sizeof(Block *));
        forall(2)
        {

            blocks[x] = (Block *)newBlock();
        }
        int numBlocks = 0;
        packRecords(blocks, records, 10, &numBlocks);
        printf("%d\n", numBlocks);
    }
    if (DEBUG4)
    {
        BTree t = newBTree(8);
        int fd = 0;
        writeToFile(argv[1], &fd);
        StringArray a = readFromFile("data.txt", &fd);
        int ffd = 0;
        writeToSortedFile(t, a, "sortedData.txt", &ffd);

        //ffd = open("sortedData.txt", O_RDONLY);

        //printf("%s\n", findRecord(t, "rrwsofsbc"));
        //deleteRecord(t, "rrwsofsbc");
        //close(ffd);
        //addRecord(t, "bbuqcleji ", "Who the fuck writes information systems in C", &ffd);
        //char* ss = findRecord(t, "bbuqcleji");

       // print_tree(t, t->root);

        deleteTree(t);
    }
    return 0;
}
