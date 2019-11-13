#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

static void freeRecord(void *data)
{
    return;
}
int main(int argc, char *argv[])
{
    printf("Welcome to Hash Map Usage\n");
    int fd;
    printf("%s is loading, please wait\n", argv[1]);

    StringArray sa = readFromFile(argv[1], &fd);

    if (sa == NULL)
    {
        printf("Could not find the file\n");
        return 0;
    }
    HashMap m = new_hashmap(printRecord, freeRecord, 15);
    writeToHashFile(m, sa, "hashData.txt");

    printf("Press 1 to insert a new key value pair\n");
    printf("Press 2 to remove a record(key, value pair)\n");
    printf("Press 3 to search for a record(key, value pair)\n");
    printf("Press 4 to exit\n");
    char c = 0;

    while (c != '4')
    {
        c = getchar();
        if (c == '1')
        {
            printf("Adding a new record to hashData.txt\n");
            char *key = calloc(100, sizeof(char));
            char *value = calloc(200, sizeof(char));
            printf("Enter the key\n");
            scanf("%s", key);
            printf("Enter the value\n");
            scanf("%s", value);
            if (containsHash(m, key))
            {
                printf("This key is already in the map\n");
                free(key);
                free(value);
            }
            else
            {
                writeRecordHash(key, value, hash(m, key) * RECORD_SIZE);
                put_hashmap(m, key, value);
                printf("Successfully added record\n");
                free(key);
                free(value);
            }
        }
        else if (c == '2')
        {
            printf("Removing a record from hashData.txt\n");
            printf("Enter the key of the record you want to remove\n");
            char *key = calloc(100, sizeof(char));
            printf("Enter the key\n");
            scanf("%s", key);

            if (containsHash(m, key))
            {
                printf("Successfully removed record\n");
                remove_from_hashmap(m, key);
            }
            else
            {
                printf("They key does not exist\n");
            }
        }
        else if (c == '3')
        {
            char *key = calloc(100, sizeof(char));
            printf("Search for a record in hashData.txt\n");
            printf("Enter the key\n");
            scanf("%s", key);
            if (containsHash(m, key) == false)
            {
                printf("The record does not exist\n");
            }
            else
            {
                Record r;
                printf("hash is %ld\n", hash(m, key)*RECORD_SIZE);
                readRecordHash(hash(m, key) * RECORD_SIZE, &r);
                printf("Found the record\n%s %s\n", r.key, r.value);
            }
        }
        else if (c == '4')
        {
            printf("Good bye\n");
            destroy_hashmap(m);
            return 0;
        }
    }

    return 0;
}