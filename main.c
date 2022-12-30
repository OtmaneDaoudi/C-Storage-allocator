// #include "allocator.h"
/*
ISSUES:
    -are we really guaranteed that malloc returnes aligned pointers
    -what should happen when no fitting chunk is found.
    */
#include<unistd.h>
#include<stdio.h> 

int main(void)
{ 
    void * f = sbrk(0); 
    printf("%p\n", f); 
    return 0; 
}