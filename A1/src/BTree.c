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
    n->beenAdded = false;

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
        insert(t, t->leaves[0], k);
        return;
    }

    //if can be inerted into a leaf
    BNode n = findLeafToInsert(t, k);
    if (canKeyBeInserted(n, k))
    {
        insert(t, n, k);
        return;
    }
    //try to borrow from a sibling if possible
   
    insert(t, n, k);
    //addLeaf(t, nn);
    //addParent(t, n, nn);
    //else you need to split the leaf and possibly internal nodes
    //find where the key should go.
    //split the leaf node.
    //insert the key into the right split
    //if there was no parent, create a parent node and add it to the array. Set the parent node's children to the
    //new split nodes, set the new split nodes parent ptr to the parent
    //else if there
}

bool borrow(BTree t, BNode n, int k, int pK)
{
    if(n == NULL || t==NULL)return false;
    BNode parent = n->parent;
    if(parent == NULL)return false;
    forall(parent->numChilden)
    {
        BNode child = parent->children[x];
        if(canKeyBeInserted(child, k))
        {
            child->keys[child->numKeys] = k;
            child->numKeys++;
            for(int i = n->numKeys-1; i>0; i--)
            {
                n->keys[i-1] = n->keys[i];
            }
            n->numKeys--;
            for(int i = 0; i < parent->numKeys; i++)
            {
                if(parent->keys[i] == pK)
                {
                    parent->keys[i] = k;
                    break;
                }
            }
            return true;
        }
    }
    return false;
}

void insert(BTree t, BNode n, int k)
{
    //print_tree(t, t->root);
    if (canKeyBeInserted(n, k))
    {
        //add the key
        n->keys[n->numKeys] = k;
        n->numKeys++;
        //sort the keys
        qsort(n->keys, n->numKeys, sizeof(int), compareInts);
        //set the root to the new node b/c it will be at lowest depth
        //if it is a leaf then add it to the leaves
        if (n->isLeaf)
        {
            addLeaf(t, n);
        }
        else
        {
            t->root = n;
            addInternal(t, n);
        }

        return;
    }

    //if the siblings have space, then you don't need to split
  
    //split the node n into n and nn
    BNode nn = split(t, n);
    nn->isLeaf = n->isLeaf;
    //continue the linked list of leaves
    if (n->isLeaf)
        n->next = nn;
    //set nn to the leaf if it is a leaf

    //right bias, add k  to nn
    insert(t, nn, k);
    //if n currently does not have a parent node then create a parent node
    if (n->parent == NULL)
    {
        BNode p = newBNode(t->m);
        p->isLeaf = false;
        //add to the internal
        insert(t, p, nn->keys[0]);
        n->parent = p;
        nn->parent = p;
        addChild(p, n);
        addChild(p, nn);
    }
    else
    {
        BNode parent = n->parent;
        insert(t, parent, nn->keys[0]);
        addChild(parent, nn);
        n->parent = parent;
        nn->parent = parent;
    }
}

BNode split(BTree t, BNode n)
{
    //set the middle element to be ceil(n->numKeys/2)
    //Create a new node which will be the right neighbor of n
    BNode nn = newBNode(t->m);
    float mid = (float)n->maxKeys/2;
    int midIndex = ceil(mid);
    //printf("max keys is %d mid index is %d\n", n->maxKeys, midIndex);
    int count = 0;

    //copy the right skewed, right half, of the keys from n to nn and remove them from n
    foreach (midIndex, n->maxKeys)
    {
        nn->keys[count] = n->keys[x];
        //default val
        n->keys[x] = -1;
        //for each key moved from n to nn, reduce the number of keys in n by 1
        n->numKeys--;
        nn->numKeys++;
        count++;
    }

    return nn;
    //add the new leaf to the tree
}


void addLeaf(BTree t, BNode n)
{
    if (n->beenAdded)
        return;
    t->leaves[t->numLeaves] = n;
    t->numLeaves++;
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
    if (p->numChilden < p->maxChildren)
    {
        p->children[p->numChilden] = n;
        p->numChilden++;
    }
}



//Searchers
BNode findLeaf(BTree t, int k)
{
    for (int i = 0; i < t->numLeaves; i++)
    {
        BNode n = t->leaves[i];
        forall(n->numKeys)
        {
            if (k == n->keys[x])
                return n;
        }
    }
    return NULL;
}
BNode findLeafToInsert(BTree t, int k)
{
    forall(t->numLeaves)
    {
        BNode n = t->leaves[x];
        if (k > n->keys[0] && k < n->keys[n->numKeys - 1])
            return n;
    }
    return t->leaves[t->numLeaves - 1];
}

//Validators
bool canKeyBeInserted(BNode n, int k)
{
    if (n->numKeys < n->maxKeys)
        return true;
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
        sprintf(temp, "%d", n->keys[x]);
        strcat(s, " ");
        strcat(s, temp);
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
    if(n == NULL)return;
    char* s = NULL;
    if(n->isLeaf)
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