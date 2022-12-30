#include<allocator.h>


block_meta_data * base = NULL; 
block_meta_data * last = NULL; //last previous visited chunk (in case no fitting chunk is found, then last is extended and then returned)
/*
TODO:
malloc allocates at least the number of bytes requested;
• The pointer returned by malloc points to an allocated space (i.e. a space where the
program can read or write successfully;)
• No other call to malloc will allocate this space or any portion of it, unless the pointer
has been freed before.
• malloc should be tractable: malloc must terminate in as soon as possible (it should not
be NP-hard !;)
• malloc should also provide resizing and freeing.
MUST RETURN ALIGNED ADRESSES
*/
void* malloc(size_t size)
{
    if(size == 0) return NULL; 
    void * mem = sbrk(size); 
    if(mem == (void *)-1) return NULL; 
    else return mem; 
}

block_meta_data * find_block(size_t size)
{
    block_meta_data * current = base;
    while(current != NULL && (!current->isFree || current->size < size))
    {
        last = current; //updated to the last visited chunk
        current = current->next; 
    }
    return current; 
}




/*
-causes a search of the free list, to find the proper place to insert the block being freed.
- If the block being freed is adjacent to a free block on either side, it is coalesced with it into a single bigger block, so storage does not become too fragmented.
*/
void free(void * ptr)
{

}