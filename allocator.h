#ifndef ALLOC_H
#define ALLOC_H

#include<stdio.h>

//in this case we consider long to be the most restrictive type
typedef long alignment; 

union freeBlock
{
    alignment a; //force alignement
    struct 
    {
        size_t size; 
        union freeBlock * next; 
        void * space; 
    }; 
};

typedef union freeBlock freeBlock; 

freeBlock* malloc(size_t);    //general-purpos storage allocator
freeBlock* morecore(size_t);  //asks the system for more memory
#endif