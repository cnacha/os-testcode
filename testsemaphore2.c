#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;

volatile long counter = 0;

void* thread(void* arg)
{
    //wait
    printf("\nBefore the wait.\n");
    sem_wait(&semaphore);
    printf("\nEntered..\n");

    //critical section
    printf("\nProcessing critical stuff..\n");
    for (int i = 0; i < 1000000000; i++)
        counter++;
    
    //signal
    printf("\nJust Exiting... %d\n", counter);
    sem_post(&semaphore);
    printf("\nAfter the post.\n");
}


int main()
{
    sem_init(&semaphore, 0, 2);
    pthread_t t1,t2;
    pthread_create(&t1,NULL,thread,NULL);
    // sleep(2);
    pthread_create(&t2,NULL,thread,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&semaphore);
    return 0;
}
