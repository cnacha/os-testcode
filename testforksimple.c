#include  <stdio.h>
#include  <sys/types.h>


void  main(void)
{
    fork();
    fork(); 
    fork();
    
    printf("I am alive here \n");
}



