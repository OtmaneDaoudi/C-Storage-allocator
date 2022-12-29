#include<stdio.h>

typedef long alignment; 

union header
{
    alignment a;  //if the most restrictive type can be stored at a particular address, all other types may be
    struct 
    {
        unsigned short size; 
        union header * next; 
    }; 
}; 

int main(void)
{
    printf("long = %d\n", sizeof(double)); 
    printf("%d\n", sizeof(union header)); 

    return 0; 
}