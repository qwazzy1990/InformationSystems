#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "BTree.h"
#include "utilities.h"
#include "math.h"

static int compareInts(const void *a, const void *b)
{
    int *f = (int *)a;
    int *s = (int *)b;
    return (*f) - (*s);
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

    n->children = calloc(m, sizeof(BNode));
    n->keys = calloc(m - 1, sizeof(int));

    n->parent = NULL;
    n->next = NULL;
    n->isLeaf = true;

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

    t->internal = calloc(1000, sizeof(BNode));
    t->leaves = calloc(1000, sizeof(BNode));

    return t;
}
/*End constructors */

void addKey(BTree t, int k)
{
    //if tree is empty
    if (t->numLeaves == 0)
    {
        t->leaves[0] = newBNode(t->m);
        t->numLeaves++;
        t->depth++;
        insert(t, t->leaves[0], k);
        return;
    }

    //if can be inerted into a leaf
    forall(t->numLeaves)
    {
        if (canKeyBeInserted(t->leaves[x], k))
        {
            insert(t, t->leaves[x], k);
            return;
        }
    }
    //else you need to split the leaf and possibly internal nodes
    //find where the key should go.
    //split the leaf node.
    //insert the key into the right split
    //if there was no parent, create a parent node and add it to the array. Set the parent node's children to the
    //new split nodes, set the new split nodes parent ptr to the parent
    //else if there
}

void addLeaf(BTree t, BNode n)
{
    t->leaves[t->numLeaves] = n;
    t->numLeaves++;
}


void insert(BTree t, BNode n, int k)
{
    if (canKeyBeInserted(n, k))
    {
        n->keys[n->numKeys] = k;
        n->numKeys++;
        qsort(n->keys, n->numKeys, sizeof(int), compareInts);
        return;
    }
}

void split(BTree t, BNode n)
{
    //set the middle element to be ceil(n->numKeys/2)
    //Create a new node which will be the right neighbor of n
    new_object(BNode, nn, 1);
    int midIndex = ceil(n->maxKeys/2);

    //copy the right skewed, right half, of the keys from n to nn and remove them from n
    foreach(midIndex, n->maxKeys)
    {
        insert(t, nn, n->keys[x]);
        //default val
        n->keys[x] = -1;
        //for each key moved from n to nn, reduce the number of keys in n by 1
        n->numKeys--;
    }
    //add the new leaf to the tree
    addLeaf(t, nn);
}

bool canKeyBeInserted(BNode n, int k)
{
    if (n->numKeys < n->maxKeys)
        return true;
    return false;
}