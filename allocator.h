#ifndef ALLOC_H
#define ALLOC_H

#include<stdio.h>

#define align4(x) ((((x-1)>>2)<<2)+4)

//already aligned
struct block_meta_data
{
    size_t size; 
    struct mem_block * next; 
    int isFree; 
}; 
typedef struct block_meta_data block_meta_data;

void* malloc(size_t);    //general-purpos storage allocator

//first fit algorithm
block_meta_data * find_block(size_t); 

void ss()
{
    printf("%p\n", sbrk(0));
}

void free(void *); 
#endif