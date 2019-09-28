#ifndef _BTREE_
#define _BTREE_

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utilities.h"

/**Node structure for node in BTree */
struct bnode
{
    //keep track of the number of children and keys in the node
    int numChilden;
    int numKeys;

    //holds min and max value for the number of keys
    int minKeys;
    int maxKeys;
    
    //holds min and max values for the number of children
    int minChildren;
    int maxChildren;

    //the array of keys in the node
    char** keys;
    //the array of children in the node
    struct bnode** children;

    //if the node is split into two new nodes, then they will have a parent
    struct bnode* parent;

    //All leaf nodes create a linked list. If the node is a leaf and has a right neighbor, then it will point 
    //to that node.
    struct bnode* next;

    //set to true if the node is a leaf
    bool isLeaf;

    bool beenAdded;

    bool toBeSplit;

};


typedef  struct bnode* BNode;

/**BTree structure */
typedef struct btree
{
    //represents the max number of children per node
    int m;
    //the root node of the tree
    BNode root;
    //keep track of number of nodes
    int numLeaves;
    int numInternal;
    int numNodes;

    //keeps track of the depth of the tree
    int depth;

    BNode* internal;
    BNode* leaves;
}btree;

typedef struct btree* BTree;


/**Constructors */
BNode newBNode( int m );
BTree newBTree( int m );

/*Setters */
void addKey(BTree t, char* key);
void add_key(BNode n, char* k);
void addLeaf(BTree t, BNode n);
void addInternal(BTree t, BNode n);
BNode insert(BTree t, BNode n, char* k);


BNode findLeafToInsert(BTree t, char* k);
BNode findLeaf(BTree t, char* k);
void splitNode(BTree t, BNode n, BNode next);


//Iterator
void sortTree(BTree t, BNode n);

//For modifying parents
void addChild(BNode p, BNode child);
void addParent(BTree t, BNode a, BNode b);
//Data represents the database address of a record
void addData(BNode n, void* data);


/*Destroyers */
void delKey(BTree t, char* key);

void shiftArray(char** a, int start, int end);

/*Validators */

bool canKeyBeInserted(BNode n, char* k);
bool sameNode(BNode n, BNode nn);
bool shouldAddChild(BTree t, BNode n);


/**PRINTERS */

char* printNode(void * data);
char* printTree(void * data);

void print_tree(BTree t, BNode n);


void deleteTree(BTree t);
void freeNode(BNode n);

#endif