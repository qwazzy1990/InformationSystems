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
#include "HashMap.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    printf("Welcome to B+ Tree Usage\n");
    printf("File is loading, please wait\n");
    int fd;

    writeToFile(argv[1], &fd);

    StringArray a = readFromFile(argv[1], &fd);

    if (a == NULL)
    {
        printf("Could not find the file\n");
        return 0;
    }
    BTree t = newBTree(8);
    fd = 0;
    int ffd = 0;
    writeToSortedFile(t, a, "sortedData.txt", &ffd);

    printf("Press 1 to search for a record by entering a key\n");
    printf("Press 2 to range search using index\n");
    printf("Press 3 to insert a new record(key and value)\n");
    printf("Press 4 to delete a record by entering a key\n");
    printf("Press 5 to display the records in order\n");
    printf("Press 6 to exit\n");
    char c = 0;

    while (c != '6')
    {
        c = getchar();
        if (c == '1')
        {
            char *key = calloc(100, sizeof(char));
            printf("Enter the key\n");
            scanf("%s", key);
            BNode n = findLeaf(t, key);
            if (n == NULL)
            {
                printf("The record does not exist\n");
            }
            else
            {
                char *s = findRecord(t, key);
                printf("%s\n", s);
                free(key);
                free(s);
            }

            //search for using a key
        }
        if (c == '3')
        {
            char *key = calloc(100, sizeof(char));
            char *value = calloc(100, sizeof(char));
            printf("Enter the key\n");
            scanf("%s", key);
            printf("Enter the value\n");
            scanf("%s", value);
            BNode n = findLeaf(t, key);
            if (n != NULL)
            {
                printf("The record already exists\n");
            }
            else
            {
                int fd;
                addRecord(t, key, value, &fd);

                char *s = findRecord(t, key);
                if(s == NULL)
                {
                    printf("Whhyyyy\n");
                    return 0;
                }
                printf("%s\n", s);
                free(key);
                free(value);
                free(s);
            }
        }

        if (c == '6')
        {
            return 0;
        }
    }
    return 0;
}