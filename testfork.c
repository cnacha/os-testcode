#include  <stdio.h>
#include  <sys/types.h>

#define   MAX_COUNT  1000

void  main(void)
{
     pid_t  pid;

     pid = fork();
     if (pid == 0) 
          ChildProcess(pid);
     else 
          ParentProcess(pid);
}

void  ChildProcess(pid_t pid)
{
     int   i;

     for (i = 1; i <= MAX_COUNT; i++)
          printf("   This line is from child %d, value = %d\n",pid, i);
     printf("   *** Child process is done ***\n");
}

void  ParentProcess(pid_t pid)
{
     int   i;

     for (i = 1; i <= MAX_COUNT; i++)
          printf("This line is from parent %d, value = %d\n",pid, i);
     printf("*** Parent is done ***\n");
}

