#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "BTree.h"
#include "utilities.h"
#include "math.h"

int compareKeys(const void *a, const void *b)
{
    if (a == NULL || b == NULL)
        printf("Whyyyyy\n");
    char **f = (char **)a;
    char **s = (char **)b;
    return strcmp((*f), (*s));
}

static int getMidIndex(BTree t)
{
    return t->m / 2;
}

static int compareNodes(const void *a, const void *b)
{
    BNode *f = (BNode *)a;
    BNode *s = (BNode *)b;

    if ((*f)->keys == NULL || (*s)->keys == NULL)
        return 0;
    return strcmp((*f)->keys[0], (*s)->keys[0]);
}

/*Constructors */
BNode newBNode(int m)
{
    new_object(BNode, n, 1);
    n->numChilden = 0;
    n->numKeys = 0;

    n->maxChildren = m;
    n->minChildren = ceil(m / 2);
    n->maxKeys = (m - 1);
    n->minKeys = ceil(m / 2) - 1;

    n->children = calloc(m + 1, sizeof(BNode));
    n->keys = calloc(10, sizeof(char *));
    forall(10)
    {
        n->keys[x] = calloc(50, sizeof(char));
    }
    n->parent = NULL;
    n->next = NULL;
    n->isLeaf = true;
    n->beenAdded = false;
    n->toBeSplit = false;
    n->deleted = false;

    return n;
}

BTree newBTree(int m)
{
    new_object(BTree, t, 1);
    t->m = m;
    t->root = NULL;
    t->depth = -1;
    t->numLeaves = 0;
    t->numInternal = 0;
    t->numNodes = 0;

    t->internal = calloc(10000000, sizeof(BNode));
    t->leaves = calloc(10000000, sizeof(BNode));

    return t;
}
/*End constructors */
void add_key(BNode n, char *k)
{
    if (n == NULL || n->keys == NULL || n->keys[n->numKeys] == NULL)
    {
        return;
    }

    strcpy(n->keys[n->numKeys], k);
    qsort(n->keys, n->numKeys + 1, sizeof(char *), compareKeys);
    n->numKeys++;
    if (n->numKeys > n->maxKeys)
    {
        n->toBeSplit = true;
    }
}

void addKey(BTree t, char *k)
{
    if(containsKey(t, k))return;
    if (t->numNodes == 0)
    {
        t->root = newBNode(t->m);
        add_key(t->root, k);
        t->numNodes++;
        sortTree(t, t->root);
    }
    else if (t->numNodes == 1)
    {
        insert(t, t->root, k);
    }
    else
    {
        BNode leaf = findLeafToInsert(t, k);
        insert(t, leaf, k);
    }
}

BNode insert(BTree t, BNode n, char *k)
{
    if (n == NULL)
    {
        n = newBNode(t->m);
        n->isLeaf = false;
        add_key(n, k);

        t->root = n;
        addInternal(t, n);
        return n;
    }
    add_key(n, k);
    if (n->toBeSplit == false)
    {

        return n;
    }
    else
    {

        BNode next = newBNode(t->m);
        splitNode(t, n, next);
        k = next->keys[0];

        BNode parent = insert(t, n->parent, k);
        n->parent = parent;
        next->parent = parent;

        addChild(parent, n);
        addChild(parent, next);
        //addInternal(t, parent);
        next->isLeaf = n->isLeaf;
        if (next->isLeaf)
        {
            addLeaf(t, n);
            n->next = next;
            addLeaf(t, next);
        }
        else
        {
            addInternal(t, next);
        }
        return next;
    }
}

void shiftArray(char **a, int start, int end)
{
    foreach (start, end - 1)
    {
        strcpy(a[x], a[x + 1]);
        free(a[x + 1]);
    }
}

void splitNode(BTree t, BNode n, BNode next)
{
    int midKeyIndex;
    int midChildIndex;
    midKeyIndex = (t->m) / 2;
    if (n->isLeaf)
    {
        foreach (midKeyIndex, t->m)
        {
            add_key(next, n->keys[x]);
        }
        foreach (midKeyIndex, t->m)
        {
            //free(n->keys[x]);
            n->numKeys--;
        }
        n->next = next;
        next->isLeaf = n->isLeaf;
    }
    else
    {
        // char *s = printNode(n);
        // printf("%s\n", s);
        // free(s);
        midChildIndex = midKeyIndex + 1;
        foreach (midChildIndex, t->m)
        {
            addChild(next, n->children[x]);
        }
        foreach (midChildIndex, t->m)
        {
            n->numChilden--;
        }
        foreach (midKeyIndex, t->m)
        {
            add_key(next, n->keys[x]);
        }
        foreach (midKeyIndex, t->m)
        {
            //free(n->keys[x]);
            n->numKeys--;
        }
        next->isLeaf = n->isLeaf;
        //print_tree(t, n);
        // print_tree(t, next);
        //print_tree(t, next);
    }
    n->toBeSplit = false;
    next->toBeSplit = false;

    qsort(n->keys, n->numKeys, sizeof(char *), compareKeys);
    qsort(n->children, n->numChilden, sizeof(BNode), compareNodes);
    qsort(next->keys, next->numKeys, sizeof(char *), compareKeys);
    qsort(next->children, next->numChilden, sizeof(BNode), compareNodes);
}

void addLeaf(BTree t, BNode n)
{
    if (n->beenAdded)
        return;
    t->leaves[t->numLeaves] = n;
    t->numLeaves++;
    t->numNodes++;
    n->beenAdded = true;
}

void addInternal(BTree t, BNode n)
{
    if (n->beenAdded)
        return;
    t->internal[t->numInternal] = n;
    t->numInternal++;
    n->beenAdded = true;
}

void addChild(BNode p, BNode n)
{
    forall(p->maxChildren)
    {
        if (p->children[x] == n)
            return;
    }
    if (p->numChilden < p->maxChildren)
    {
        p->children[p->numChilden] = n;
        p->numChilden++;
        return;
    }
    if (p->parent != NULL)
    {
        BNode gParent = p->parent;
        forall(gParent->numChilden)
        {
            BNode parent = gParent->children[x];
            if (parent->numChilden < parent->maxChildren)
            {
                parent->children[parent->numChilden] = n;
                parent->numChilden++;
                gParent->children[x] = parent;
            }
        }
    }
}

//Searchers
BNode findLeaf(BTree t, char *k)
{
    for (int i = 0; i < t->numLeaves; i++)
    {
        BNode n = t->leaves[i];
        forall(n->numKeys)
        {
            if (strcmp(k, n->keys[x]) == 0)
                return n;
        }
    }
    return NULL;
}
BNode findLeafToInsert(BTree t, char *k)
{
    if (t->numLeaves == 0)
        return t->root;
    forall(t->numLeaves)
    {
        BNode n = t->leaves[x];
        if (strcmp(k, n->keys[0]) > 0 && strcmp(k, n->keys[n->numKeys - 1]) < 0)
        {
            return n;
        }
    }
    return t->leaves[t->numLeaves - 1];
}

void sortTree(BTree t, BNode n)
{
    if (n == NULL)
    {
        return;
    }
    if (n->isLeaf)
    {
        qsort(n->keys, n->numKeys, sizeof(char *), compareKeys);
        return;
    }
    qsort(n->keys, n->numKeys, sizeof(char *), compareKeys);
    qsort(n->children, n->numChilden, sizeof(BNode), compareNodes);
    forall(n->numChilden)
    {
        sortTree(t, n->children[x]);
    }
}

//Validators
bool canKeyBeInserted(BNode n, char *k)
{
    if (n->numKeys <= n->maxKeys)
        return true;
    return false;
}

bool sameNode(BNode n, BNode nn)
{
    char *s1 = printNode(n);
    char *s2 = printNode(nn);
    if (strcmp(s1, s2) == 0)
    {
        free(s1);
        free(s2);
        return true;
    }
    free(s1);
    free(s2);
    return false;
}

bool containsKey(BTree t, char* key)
{
    if(t == NULL || key == NULL)return false;
    forall(t->numLeaves)
    {
        BNode n = t->leaves[x];
        for(int i = 0; i < n->numKeys; i++)
        {
            char* k = n->keys[i];
            if(strcmp(k, key)==0)return true;
        }
    }
    return false;
}

/**PRINTERS */

char *printNode(void *data)
{
    BNode n = (BNode)data;
    if (n == NULL)
        return NULL;

    char *s = calloc(1000, sizeof(char));
    strcpy(s, "\n---BNode---\n");
    char temp[1000];
    strcat(s, "Keys: ");
    forall(n->numKeys)
    {

        strcat(s, " ");
        strcat(s, n->keys[x]);
    }
    strcat(s, "\n");
    if (n->isLeaf)
    {
        strcat(s, "Is Leaf\n");
    }
    else
    {
        strcat(s, "Is not leaf\n");
    }
    if (n->parent != NULL)
    {
        strcat(s, "Has a parent\n");
    }
    else
    {
        strcat(s, "Has no parent\n");
    }
    sprintf(temp, "%d", n->numChilden);
    strcat(s, "Number of children: ");
    strcat(s, temp);
    return s;
}
char *printTree(void *data)
{
    BTree t = (BTree)data;
    if (t == NULL)
        return NULL;
    char *s = calloc(1000, sizeof(char));
    char *temp = NULL;
    strcat(s, "\n----ROOT----\n");
    temp = printNode(t->root);
    strcat(s, temp);
    free(temp);
    strcat(s, "\n---LEAVES---\n");
    forall(t->numLeaves)
    {

        temp = printNode(t->leaves[x]);
        s = strcat(s, temp);
        free(temp);
    }
    strcat(s, "\n");
    strcat(s, "\n---INTERNAL---\n");
    forall(t->numInternal)
    {
        temp = printNode(t->internal[x]);
        strcat(s, temp);
        free(temp);
    }
    return s;
}

void print_tree(BTree t, BNode n)
{
    if (n == NULL)
        return;
    char *s = NULL;
    if (n->isLeaf)
    {
        s = printNode(n);
        printf("%s\n", s);
        free(s);
        return;
    }
    s = printNode(n);
    printf("%s\n", s);
    free(s);
    forall(n->numChilden)
    {
        print_tree(t, n->children[x]);
    }
}


void deleteTree(BTree t)
{
    forall(t->numLeaves)
    {
        freeNode(t->leaves[x]);

    }
    forall(t->numInternal)
    {
        free(t->internal[x]);
    }
    free(t->leaves);
    free(t->internal);
    free(t);

}


void freeNode(BNode n)
{
    if(n == NULL)return;
    forall(10)
    {
        if(n->keys[x] != NULL){
            free(n->keys[x]);
            n->keys[x] = NULL;
        }
    }
    free(n->keys);
    free(n->children);
    free(n);
}
