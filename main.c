#include "allocator.h"

int main(void)
{ 
    int* arr = (int*)malloc(sizeof(int)); 
    *arr = 5; 
    printf("%p %d\n", arr, *arr);

    double* arr2 = (double*)malloc(sizeof(double) * 4); 
    for(int i=0; i<4; i++)
    {
        *(arr2 + i) = (10.5f) + i; 
        printf("%p %lf\n", (arr2 + i), *(arr2 + i)); 
    }
    return 0; 
}