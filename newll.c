#include<stdio.h>
#include<unistd.h>

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

list blocks_list = {NULL, NULL}; 


int main(void)
{
    char a[] = "hello"; 
    printf("%s\n", a); 
    printf("base = %p | end = %p\n", blocks_list.base,blocks_list.end); 
    return 0;  
} 