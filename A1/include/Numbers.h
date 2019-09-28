/**
 * NAME: Patrick Di Salvo
 * 0629465
 * */


#ifndef _NUMBERS_
#define _NUMBERS_


//#include "DynamicString.h"
#include "utilities.h"



struct intt
{
    long long* ints;
    int size;

    void (*concat)(struct intt*, struct intt*);
    void (*add)(struct intt*, int, ...);
    void (*remove)(struct intt*, long long);
    void (*remove_at)(struct intt*, long long);
    void (*remove_all)(struct intt*);
    void (*remove_smallest)(struct intt*);
    void (*remove_largest)(struct intt*);
    void (*destroy)(void*);

    char* (*print)(void*);
    void (*sort)(struct intt*);

};
typedef struct intt Intt;

typedef Intt* Int;

struct doub
{
    long double* doubles;
    int size;

    void (*add)(struct doub*, double);
    void (*concat)(struct doub* dest, struct doub* source);
    void (*remove)(struct doub*, long double);
    void (*remove_at)(struct doub*, long long);
    void (*remove_smallest)(struct doub*);
    void (*remove_largest)(struct doub*);
    void (*remove_all)(struct doub*);
    void (*destroy)(void*);
    
    char* (*print)(void*);
    void (*sort)(struct doub*);

};
typedef struct doub Doub;
typedef Doub* Double;

Int new_int( void );

void concat_int(Int dest, Int source);


Double new_double( void );
void concat_double(Double dest, Double source);


void add_int(Int m, int n, ...);

void add_double(Double d, double n);


void remove_int(Int m, long long n);
void remove_int_at(Int m, long long n);
void remove_all_ints(Int m);
void remove_smallest_int(Int m);
void remove_largest_int(Int m);
void destroy_int(void* m);

void remove_double(Double d, long double n);
void remove_double_at(Double d, long long n);
void remove_all_doubles(Double d);
void remove_smallest_double(Double d);
void remove_largest_double(Double d);
void destroy_double(void* d);

char* print_int(void* m);

char* print_double(void* d);




void sort_int(Int m);

void sort_double(Double d);

#endif