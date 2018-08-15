#include <stdio.h>
int main(){
	int *ptr,q;
	q = 50;
	ptr = &q;
	printf("%d \n",*ptr);
	printf("%d \n",ptr);
	return 0;
} 
