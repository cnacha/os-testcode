/* 
 * File:   dispatchQueue.h
 * Author: robert
 *
 * Modified on August 7, 2018
 */


#ifndef DISPATCHQUEUE_H
#define	DISPATCHQUEUE_H

#ifdef	__cplusplus
extern "C" {
#endif


#include <pthread.h>
#include <semaphore.h>
    
#define error_exit(MESSAGE)     perror(MESSAGE), exit(EXIT_FAILURE)

    typedef enum { // whether dispatching a task synchronously or asynchronously
        ASYNC, SYNC
    } task_dispatch_type_t;
    
    typedef enum { // The type of dispatch queue.
        CONCURRENT, SERIAL
    } queue_type_t;
    
    typedef enum { // whether a queue is running, being waited on, or shutting down
        RUN, WAITED_ON, SHUTTING_DOWN
    } queue_state_t;

    typedef struct task {
        char name[64];          // to identify it when debugging
        void (*work)(void *);   // the function to perform
        void *params;           // parameters to pass to the function

    } task_t;
    
  

#ifdef	__cplusplus
}
#endif

#endif	/* DISPATCHQUEUE_H */
