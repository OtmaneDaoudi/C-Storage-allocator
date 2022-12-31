#include "allocator.h"

list blocks_list = {.base = NULL, .end = NULL};  

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
    printf("split block\n"); 
    //assuming that the block fits fo the requested size , and the passed
    //requested size is multiple of four
    char* bytePointer = (char*)block; 
    block_meta_data* nextBlock = block->next; 
    block->next = (block_meta_data*)(bytePointer + BLOCK_SIZE + requestedSize); 
    block->next->size = block->size - requestedSize - BLOCK_SIZE; 
    block->next->next = nextBlock; 
    block->next->isFree = 1; 
    block->size = requestedSize; 
    // printf("block at %p was split into two blocks : \n", block); 
    // printf("\tblock1 at %p with size = %lu\n", block, block->size); 
    // printf("\tblock2 at %p with size = %lu\n", block->next, block->next->size); 
}


void* malloc(size_t size)
{
    if(size == 0) return NULL; 
    size_t alignedSize = align4(size); 
    //test for first call
    block_meta_data* chunk = find_block(alignedSize);     
    if(chunk != NULL) 
    {
        //chunk found
        //check for splits
        size_t availableSpace = chunk->size - alignedSize - BLOCK_SIZE; 
        if(availableSpace >= 4)
        {
            //chunk is splitable AND the remaining chunk is aligned
            splitBlock(chunk, alignedSize); 
        }
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


/*
-causes a search of the free list, to find the proper place to insert the block being freed.
- If the block being freed is adjacent to a free block on either side, it is coalesced with it into a single bigger block, so storage does not become too fragmented.
*/
void free(void * ptr)
{

}