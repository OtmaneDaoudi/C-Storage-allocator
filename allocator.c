#include "allocator.h"

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
    block->next->isFree = 1; 
    block->size = requestedSize; 

    //update last block in the list
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
    if(next != NULL && next->isFree) mergeBlocks(block, next); //merge with next
}

void free_v2(void* mem)
{
    if(mem == NULL) return; 
    //assuring that mem is previously allocated by malloc_v2
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