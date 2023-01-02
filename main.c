#include<stdio.h>
#include<unistd.h>
#include<string.h>


//function for sizes that yield an aligned pointers (4 bytes alignemment)
//given a positive integer , it will return the next multiple of 4
#define align4(x) ((((x-1)>>2)<<2)+4)
#define BLOCK_SIZE sizeof(block_meta_data)

//already aligned
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

//a new chunk is allocated (becomes the new tail) if no previous chunk fits
void* malloc_v2(size_t);    //general-purpos storage allocator

//first fit algorithm   
block_meta_data* find_block(size_t); 

//extends the heap if no fiting chunk is found
int extendHeap(size_t); 

//
void splitBlock(block_meta_data*, size_t);

void free_v2(void*);

void* calloc_v2(size_t, size_t); 

//a function that deals with memory fragmentation
void nofrag(block_meta_data*, block_meta_data*); 

//merge both blocks in the first block
void mergeBlocks(block_meta_data*, block_meta_data*); 
//===============================================================
//===============================================================


list blocks_list = {NULL, NULL}; 

block_meta_data* find_block(size_t size)
{
    block_meta_data* current = blocks_list.base; 
    while(current != NULL && (!current->isFree || current->size < size)) 
    {
        current = current->next;
    }  
    return current; 
}

int extendHeap(size_t size)
{
    //size is aligned
    void* mem; 
    if((mem = sbrk(BLOCK_SIZE + size)) == (void*)-1) return 0; //sbrk fails
    
    block_meta_data* newBlock = (block_meta_data*)mem; 
    newBlock->size = size;
    newBlock->next = NULL;
    newBlock->isFree = 0; 
    //check for first-time allocation
    if(blocks_list.end == NULL) blocks_list.base = blocks_list.end = newBlock;  
    else
    {
        blocks_list.end->next = newBlock; 
        blocks_list.end = newBlock; 
    }
    // printf("heap extended by creating a new block at %p of size = %lu.\n", newBlock, newBlock->size);
    return 1; 
}


void splitBlock(block_meta_data* block, size_t requestedSize)
{
    //assuming that the block fits the requested size , and the passed requested size is aligned
    char* bytePointer = (char*)block; 
    block_meta_data* nextBlock = block->next; 
    block->next = (block_meta_data*)(bytePointer + BLOCK_SIZE + requestedSize); 
    block->next->size = block->size - requestedSize - BLOCK_SIZE; 
    block->next->next = nextBlock; 
    // block->isFree = 0; 
    block->next->isFree = 1; 
    block->size = requestedSize; 

    //last block in the list
    if(nextBlock == NULL) blocks_list.end = block->next; 
}

void* malloc_v2(size_t size)
{
    if(size == 0) return NULL; 
    size_t alignedSize = align4(size); 
    //test for first call
    block_meta_data* chunk = find_block(alignedSize);     
    if(chunk != NULL) 
    {
        //chunk found
        //check for splits
        int availableSpace = (int)chunk->size - (int)alignedSize - BLOCK_SIZE; 
        if(availableSpace >= 4)
        {
            //chunk is splitable AND the remaining chunk is aligned
            splitBlock(chunk, alignedSize); 
        }
        chunk->isFree = 0;
        return ((void*)chunk) + BLOCK_SIZE;   
    }
    else
    {
        //no chunk is found ==> extend the heap
        if(!extendHeap(alignedSize))
        {
            fprintf(stderr, "Error while trying to extend the heap.\n"); 
        }
        return ((char*)blocks_list.end) + BLOCK_SIZE; 
    }
}

void showBlock()
{
    block_meta_data* current = blocks_list.base;
    int i = 0; 
    while(current != NULL)
    {
        printf("block %d at %p :\n", i++, current);   
        printf("\tsize = %lu | isFree = %d | next = %p\n", current->size, current->isFree, current->next); 
        current = current->next; 
    }
    printf("\n"); 
}

void* calloc_v2(size_t numberOfElements, size_t sizeOfEach)
{
    void* mem = malloc_v2(numberOfElements * sizeOfEach); 
    //initialize the whole block to 0s
    size_t bytes = align4(numberOfElements * sizeOfEach); 
    for(int i=0; i<bytes; i++) *(((char*)mem) + i) = 0; 
    return mem; 
}

void mergeBlocks(block_meta_data* target, block_meta_data* block)
{
    target->next = block->next; 
    target->size = target->size + BLOCK_SIZE + block->size;
}

void nofrag(block_meta_data* block, block_meta_data* prev)
{
    //block is assured to be free at this point
    block_meta_data* next = block->next; 
    if(prev != NULL && prev->isFree)
    {
        //merge with prev
        mergeBlocks(prev, block); 
        block = prev; 
    }
    if(next != NULL && next->isFree) mergeBlocks(block, next); 
}

void free_v2(void* mem)
{
    if(mem == NULL) return; 
    //assure that mem is previously allocated by malloc_v2
    block_meta_data* block = (block_meta_data*)(((char*)mem) - BLOCK_SIZE);
    int isAllocated = 0; 
    block_meta_data* current = blocks_list.base, *prev = NULL; 
    while(current != NULL)
    {
        if(block == current)
        {
            isAllocated = 1; 
            break; 
        }
        prev = current; 
        current = current->next; 
    }
    if(isAllocated)
    {
        if(!block->isFree) 
        {
            block->isFree = 1;
            nofrag(block, prev); 
        }
        else printf("Info : the block is already free_v2 and available for storage.\n");  
    }
    else
    {
        fprintf(stderr, "error free_v2ing a block that is not previously allocated by malloc_v2.\n"); 
    }
}

int main(void)
{
    // char * mem = (char*)malloc_v2(sizeof(char) * 56); 
    // free_v2(mem);
    // char* string20 = (char*)malloc_v2(sizeof(char) * 20);
    // // showBlock();
    // strcpy(string20, "hello world");   
  
    // char* string12 = (char*)malloc_v2(sizeof(char) * 12);
    // strcpy(string12, "otmane XXX"); 
    double* arr = (double*)malloc_v2(sizeof(double) * 20); 
    for(int i=0; i<20; i++) *(arr + i) = i; 
    showBlock(); 
    free_v2(arr); 
    showBlock(); 
    void* ptr[6]; 
    for(int i=0; i<6; i++) ptr[i] = malloc_v2(sizeof(int));
    showBlock();    
    int* c = (int*)malloc_v2(sizeof(int)); 
    showBlock(); 

    free_v2(ptr[0]); 
    // free_v2(ptr[1]); 
    showBlock();
    free_v2(ptr[1]); 
    showBlock(); 
    free_v2(ptr[2]); 
    showBlock(); 

    // double* d = (double*)malloc_v2(sizeof(double)); 
    // showBlock(); 
    // free_v2(d); 
    // showBlock(); 
    return 0; 
}