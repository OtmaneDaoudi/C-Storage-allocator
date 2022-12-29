#include<allocator.h>

freeBlock * head = NULL;

/*
-last block always points to the first
-first fit approach
-If the block is exactly the size requested it is unlinked from the list and returned to the user

*/
freeBlock* malloc(size_t size)
{
    if(size == 0) return NULL; 

}