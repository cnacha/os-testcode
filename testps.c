#include <stdlib.h>
#include <unistd.h>

int
main()
{
  //system("ps");
  int pid;
  for (int count = 0; count < 3; ++count) {
    pid = fork();
    if (pid == 0) {
	printf("child %d\n",pid);      
	//system("ps");
    } else
	printf("parent %d\n",pid); 
  }

  return 0;
}

