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
    printf("%s is loading, please wait\n", argv[1]);
    int fd;

    StringArray a = readFromFile(argv[1], &fd);

    if (a == NULL)
    {
        printf("Could not find the file\n");
        return 0;
    }
    BTree t = newBTree(8);
    fd = 0;
    int ffd = 0;
    writeToSortedFile(t, a, "TreeData.txt", &ffd);

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
            printf("Searching for a record in TreeData.txt\n");
            char *key = calloc(100, sizeof(char));
            printf("Enter the key to search for\n");
            scanf("%s", key);
            toLower(key);

            char *s = findRecord(t, key);
            if (s == NULL)
            {
                printf("Could not find record\n");
                return 0;
            }
            if (strlen(s) == 0)
            {
                printf("Could not find record\n");
            }
            else
            {
                printf("%s\n", s);
                free(key);
                free(s);
            }

            //search for using a key
        }

        if (c == '2')
        {
            printf("Performing a range search in TreeData.txt\n");
            char *k1 = calloc(100, sizeof(char));
            char *k2 = calloc(100, sizeof(char));
            printf("Enter the first key\n");
            scanf("%s", k1);
            printf("Enter the second key\n");
            scanf("%s", k2);

            char *s = rangeSearch(t, k1, k2);
            free(k1);
            free(k2);
            if (s == NULL)
            {
                printf("The range searched failed\n");
            }
            else
            {
                printf("%s\n", s);
                free(s);
            }
        }
        if (c == '3')
        {
            printf("Adding a record to TreeData.txt\n");
            char *key = calloc(100, sizeof(char));
            char *value = calloc(100, sizeof(char));
            printf("Enter the key to add\n");
            //get the user key
            scanf("%s", key);
            toLower(key);

            //get the user value
            printf("Enter the value to add\n");
            strcpy(value, "  ");
            scanf("%s", value);

          
                int fd;
                addRecord(t, key, value, &fd);

                char *s = findRecord(t, key);
                if (s == NULL)
                {
                    printf("Error adding the key\n");
                }
                printf("%s\n", s);
                free(key);
                free(value);
                free(s);
        
        }

        if (c == '4')
        {
            printf("Deleting a record from TreeData.txt\n");
            printf("Enter the key to delete\n");
            char *key = calloc(100, sizeof(char));
            scanf("%s", key);
            char *record = findRecord(t, key);
            if (record == NULL)
            {
                printf("The record does not exist\n");
                free(key);
            }
            else
            {
                printf("Successfully deleted record\n");
                deleteRecord(t, key);
                free(key);
                free(record);
            }
        }

        if (c == '5')
        {
            printf("Printing all the records in TreeData.txt\n");
            print_tree(t, t->root);
        }

        if (c == '6')
        {
            printf("Good bye\n");
            deleteTree(t);
            return 0;
        }
    }
    return 0;
}