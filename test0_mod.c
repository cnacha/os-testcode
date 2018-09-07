// Author:  Robert Sheehan
#include "dispatchQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;

void test0() {
    sleep(1);
    printf("test running\n");
}

/* In the submitted version a function like this would be in 
   your dispatchQueue.c file. */
task_t *task_create(void (*work)(void *), void *params, char *name) {
    task_t *task = (task_t *)malloc(sizeof (task_t));
    task->work = work;
    task->params = params;
    strcpy(task->name, name);
    return task;
}

void *dummy_runner(void *run_task) {
    task_t *my_task = (task_t *)run_task;
    sem_wait(&mutex);
    // critical section begins here
    printf("starting dummy_runner on %s\n", my_task->name);
    my_task->work(NULL); /* possibly some parameters later */
    printf("finished dummy_runner on %s\n", my_task->name);
    // critical section ends here
    sem_post(&mutex);
}

int main(int argc, char** argv) {
    sem_init(&mutex, 0, 1);

    task_t *task1, *task2;
    pthread_t test_thread1, test_thread2;

    task1 = task_create(test0, NULL, "test0");
    task2 = task_create(test0, NULL, "test1");

    printf("Before\n");
    pthread_create(&test_thread1, NULL, (void *)dummy_runner, (void *)task1);
    pthread_create(&test_thread2, NULL, (void *)dummy_runner, (void *)task2);
   
    printf("After\n");
    pthread_join(test_thread1, NULL);
    pthread_join(test_thread2, NULL);
    return EXIT_SUCCESS;
}
