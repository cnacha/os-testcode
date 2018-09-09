// C program to demonstrate function pointer
#include <stdio.h>

int addInt(int n, int m) {
    return n+m;
}

int callFunc(int (*functionPtr)(int, int)) {
    return (*functionPtr)(2, 3);
}


int main()
{
    int (*functionPtr)(int,int);
    functionPtr = &addInt;
    int sum = callFunc(functionPtr); //(*functionPtr)(2, 3); 
    printf("output is %d \n",sum);
}

