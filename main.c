#include<stdio.h>
#include<unistd.h>

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
void* malloc(size_t);    //general-purpos storage allocator

//first fit algorithm   
block_meta_data* find_block(size_t); 

//extends the heap if no fiting chunk is found
int extendHeap(size_t); 

//
void splitBlock(block_meta_data*, size_t);

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
    if((mem = sbrk(BLOCK_SIZE + size*10)) == (void*)-1) return 0; //sbrk fails
    
    block_meta_data* newBlock = (block_meta_data*)mem; 
    newBlock->size = size*10;
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

void showBlock()
{
    block_meta_data* current = blocks_list.base;
    int i = 0;
    printf("block %d at %p :\n");   
    while(current != NULL)
    {
        printf("\tsize = %d | isFree = %d | next = %p\n", current->size, current->isFree, current->next); 
        current = current->next; 
    }
}

int main(void)
{ 
    int* arr = (int*)malloc(sizeof(int)); 
    *arr = 5; 
    printf("%p %d\n", arr, *arr);

    int* x = (int*)malloc(sizeof(int)); 
    *x = 58; 
    printf("%p %d\n", x, *x); 

    showBlock(); 
    
    return 0; 
}