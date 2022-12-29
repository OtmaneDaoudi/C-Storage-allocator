#include<allocator.h>


/*
-last block always points to the first
-first fit approach
-If the block is exactly the size requested it is unlinked from the list and returned to the user
-If the block is too big, it is split, and the proper amount is returned to the user while the residue remains on the free list
-If no big-enough block is found, another large chunk is obtained by the operating system and linked into the free list.
-To simplify alignment, all blocks are multiples of the header size
-the requested size in characters is rounded up to the proper number of header-sized units
*/
void* malloc(size_t size)
{
    if(size == 0) return NULL; 
    void * mem = sbrk(size); 
    if(mem == (void *)-1) return NULL; 
    else return mem; 
}
/*
-causes a search of the free list, to find the proper place to insert the block being freed.
- If the block being freed is adjacent to a free block on either side, it is coalesced with it into a single bigger block, so storage does not become too fragmented.
*/
void free(void * ptr)
{

}