#include "allocator.h"

int main(void)
{
    double * a = (double*)malloc(sizeof(double)); 
    *a = 50; 
    printf("a = %lf\n", *a); 

    return 0; 
}