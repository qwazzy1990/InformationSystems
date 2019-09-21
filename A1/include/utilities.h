#ifndef _UTILITIES_
#define _UTILITIES_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#define _MAIN_ int main(int agrc, char* argv[])




#define INC_ASSER 
#define clear(myarg)            \
	clear_real((void *)&myarg); \
	myarg = NULL

/** DEBUGGING AND PRINTING MACRO**/
#define DEBUGPRINT false
#define P           \
	if (DEBUGPRINT) \
	printf

/***CONSTRUCTORS***/
#define new (type, myarg, size) type *myarg = calloc(size, sizeof(*myarg))

#define new_object(type, myarg, size) type myarg = calloc(size, sizeof(*myarg))

/***MEMORY ALLOCATERS***/
#define mem_alloc(myarg, size) myarg = calloc(size, sizeof(*myarg))

#define resize(myarg, size) myarg = realloc(myarg, 1000);

/******DESTROYERS FOR SINGLE AND DOUBLE POINTERS TO ANY DATA****/

/****CONDITIONALS****/

#define ifandtrue(a) if(a==true)
#define ifortrue(a) if(a == true)
#define ifandfalse(a) if(a == false)
#define iforfalse(a) if(a == false)

/**ITERATORS**** Do not use a variable 'x' in block of code when using iterator. Will cause unpredicted beghavior**/
#define foreach(index, number) for (int x = index; x < number; x++)

#define forall(number) for (int x = 0; x < number; x++)

#define doublefor(number)                \
	for (int x = 0; x < number - 1; x++) \
		for (int y = x + 1; y < number; y++)

//MACRO TO GET THE SIZE OF THE DATA THAT A POINTER IS POINTING TO
#define SIZE(x) sizeof(*x)

/***SIZES OF PRIMITIVES***/
#define CHAR_SIZE 1
#define INT_SIZE 4
#define FLOAT_SIZE 8
#define PTR_SIZE 8
#define LONG_SIZE 8
#define DOUBLE_SIZE 8

/***typedefs for pointers to primitives with exception of char* which can be found in String.h***/


typedef void *AnyData;


enum type
{
	INT,
	CHAR,
	BOOL,
	DOUBLE,
	LONG,
	FLOAT,
	UNSIGNED_INT
};
typedef enum type Primitive;


enum lessequal
{
	LESS_THAN,
	EQUAL
};
typedef enum lessequal LessOrEqual;


/**
		 *Typedefs for function pointers for standard functions
		   pointer to swap function, compare function, print function and delete function
		 *
		 * **/
typedef void (*SwapFunc)(AnyData a, AnyData b);
typedef int (*CompareFunc)(AnyData a, AnyData b);
typedef char *(*PrintFunc)(AnyData data);
typedef void (*DeleteFunc)(AnyData data);
typedef void *(*CloneFunc)(AnyData data);

/**
		 *Struct for containining CloneFunction pointer. 
		 *User must provide a string representing the type 
		 of data that will be cloned. The String "type" should 
		 be the same as the data the user wants to clone.
		 * **/

struct cl
{
	CloneFunc cloneData;
};

typedef struct cl Cloner;

/**
		 * Swapping functions
		 * **/
void swap(AnyData a, AnyData b, SwapFunc s);

void swapInts(AnyData a, AnyData b);

void swapChars(AnyData a, AnyData b);

void swapDoubles(AnyData a, AnyData b);

/**
		 * Comparing Functions
		 * **/

int compare(AnyData a, AnyData b, CompareFunc c);

/*******MISCECLLANIOUS FUNCTIONS*******/
void remove_hard_return(char *s);

bool sameobject(AnyData a, AnyData b);

/********TESTING CLONE FUNCTION POINTER******/

/**
 * If a function returns a non void* then a function pointer to a function that returns a void* cannot be assigned the address of the function
 * that returns a non void* Likewise if the parameter to a function pointer is a void* a function that has a parameter of a non void* 
 * cannot be assigned to a variable of type void*(*foo)(void*)
 * **/
int *test_clone(void *a);

/**
 * Function to create a Cloner object.
 * Any clone function passed to cloner must return a void*.
 * Therefore it is expected that the user type cast the object created 
 * in clone function to a void* before returning the object from clone 
 * function.
 * The type parameter represent the struct/type of object returned from 
 * the clonerFunction assuming it were NOT type-cast to void.
 * Example of how to create a Cloner with a clone function
 * And a char* type
 *  Assume there is a type int*.
 * 
 * 	The Cloning Function must follow this declaration and definition
 * 
 *   void* clone_int(void* original){
 *      int* temp = (int*)original;
 * 		int* clone = calloc(1, sizeof(*clone));
 * 		*clone = *temp;
 *		return (void*)clone; 
 *
 *   }end function
 * 
 *  
 * 
 *   Cloner newCloner = new_cloner(clone_int);
 *  
 * */
Cloner new_cloner(CloneFunc clonerFunction);

void destroy_cloner(Cloner);

/**************MEMORY ALLOCATERS*********/

void resize_real(AnyData *a, int size);

/*********DESTROYERS*******/

void clear_real(AnyData d);

/****SORTERS***/

void quick_sort(void *array, int left, int right, CompareFunc compare);

/******DUMMY FUNCTIONS: DEFAULT TYPE IS int* ****/

int dummy_compare(const void *a, const void *b);

char *dummy_print(AnyData p);

void dummy_delete(AnyData d);



bool less_than(const double a, const double b);

bool greater_than(const double a, const double b);

#endif
