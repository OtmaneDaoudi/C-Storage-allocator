#include<allocator.h>

list blocks_list = {.base = NULL, .end = NULL};  

void* malloc(size_t size)
{
    if(size == 0) return NULL; 
    void * mem ; //= sbrk(size); 
    if(mem == (void *)-1) return NULL; 
    else return mem; 
}

block_meta_data* find_block(size_t size)
{
    block_meta_data* current = blocks_list.base;
    while(current != NULL && (!current->isFree || current->size < size)) 
        current = current->next; 
    return current; 
}

block_meta_data* extendHeap(size_t size)
{
    //last is updated
    void* mem; 
    if((mem = sbrk(BLOCK_SIZE + size)) == (void*)-1) return NULL; //sbrk fails
    blocks_list.end->next = (block_meta_data*)mem; 
    blocks_list.end = blocks_list.end->next; 
    blocks_list.end->size = size; 
    blocks_list.end->next = NULL; 
    blocks_list.end->isFree = 1; 

    return blocks_list.end; 
}


/*
-causes a search of the free list, to find the proper place to insert the block being freed.
- If the block being freed is adjacent to a free block on either side, it is coalesced with it into a single bigger block, so storage does not become too fragmented.
*/
void free(void * ptr)
{

}