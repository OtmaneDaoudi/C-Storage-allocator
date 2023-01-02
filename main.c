#include "allocator.h"

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

    return 0; 
}