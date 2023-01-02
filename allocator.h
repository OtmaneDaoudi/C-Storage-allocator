#ifndef ALLOC_H
#define ALLOC_H

#include<stdio.h>
#include<unistd.h>
#include<string.h>

//function for sizes that yield an aligned pointers (4 bytes alignemment)
//given a positive integer , it will return the next multiple of 4
#define align4(x) ((((x-1)>>2)<<2)+4)
#define BLOCK_SIZE sizeof(block_meta_data)

struct block_meta_data
{
    size_t size; 
    struct block_meta_data * next; 
    int isFree; 
}; 
typedef struct block_meta_data block_meta_data;

struct list
{
    block_meta_data* base; 
    block_meta_data* end; 
}; 
typedef struct list list;

//general-purpos storage allocator
//a new chunk is allocated (becomes the new tail) if no previous chunk fits
void* malloc_v2(size_t);    

//first fit algorithm   
block_meta_data* find_block(size_t); 

//extends the heap if no fiting chunk is found
int extendHeap(size_t); 

void splitBlock(block_meta_data*, size_t);

void free_v2(void*);

void* calloc_v2(size_t, size_t); 

//a function that deals with memory fragmentation
void nofrag(block_meta_data*, block_meta_data*); 

//merge both blocks in the first block
void mergeBlocks(block_meta_data*, block_meta_data*); 

#endif