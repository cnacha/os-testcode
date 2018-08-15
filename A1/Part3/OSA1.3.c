/*
 ============================================================================
 Name        : OSA1.c
 Author      : Robert Sheehan
 Version     : 1.0
 Description : Single thread implementation.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


#include "littleThread.h"
#include "threads3.c" // rename this for different threads

Thread newThread; // the thread currently being set up
Thread mainThread; // the main thread
Thread currentThread;
Thread orderedThreadList[100];
int numThreads = 0;
struct sigaction setUpAction;

struct sigaction timerAction;

struct itimerval timer;

/*
 * Switches execution from prevThread to nextThread.
 */
void switcher(Thread prevThread, Thread nextThread) {
	currentThread = nextThread;
	if (prevThread->state == FINISHED) {
		nextThread->state = RUNNING;
		printf("\ndisposing %d\n", prevThread->tid);
		free(prevThread->stackAddr); // just thrown away my own stack
		printThreadStates();
		longjmp(nextThread->environment, 1);
	} else if (setjmp(prevThread->environment) == 0) { // so we can come back here
		prevThread->state = READY;
		nextThread->state = RUNNING;
		printThreadStates();
		longjmp(nextThread->environment, 1);
	}
}

char *threadState(enum state_t state) {
	char *result;
	switch (state) {
	case SETUP:
		result = "setup";
		break;
	case RUNNING:
		result = "running";
		break;
	case READY:
		result = "ready";
		break;
	case FINISHED:
		result = "finished";
		break;
	}
	return result;
}


void printThreadStates() {
	int i;
	puts("\nThread States\n=============");
	for (i = 0; i < numThreads; i++)
		printf("threadID: %d prev:%d next:%d  state:%s\n", orderedThreadList[i]->tid,  orderedThreadList[i]->prev->tid,  orderedThreadList[i]->next->tid,
						threadState(orderedThreadList[i]->state));
	puts("");
}


void scheduler(){
	
	
	printf("scheduling %d -> %d\n",currentThread->tid,currentThread->next->tid);
	if(currentThread->next == currentThread){
		free(currentThread->stackAddr);		
		longjmp(mainThread->environment, 1); // escapes if no more threads
	} else {
		printf("going to next thread");
		switcher(currentThread, currentThread->next);	
	}
}

void removeThread(){
	currentThread->state = FINISHED;
	//NACHA remove thread from list
	printf("removing prev of %d -> %d\n",currentThread->next->prev->tid,currentThread->prev->tid);
	printf("removing next of %d -> %d\n",currentThread->prev->next->tid,currentThread->next->tid);
	currentThread->next->prev = currentThread->prev;
	currentThread->prev->next = currentThread ->next;
	scheduler();
}

/*
 * Associates the signal stack with the newThread.
 * Also sets up the newThread to start running after it is long jumped to.
 * This is called when SIGUSR1 is received.
 */
void associateStack(int signum) {
	Thread localThread = newThread; // what if we don't use this local variable?
	localThread->state = READY; // now it has its stack
	if (setjmp(localThread->environment) != 0) { // will be zero if called directly
		(localThread->start)();
		removeThread(); // NACHA schedule to the next thread
	}
}

/*
 * Sets up the user signal handler so that when SIGUSR1 is received
 * it will use a separate stack. This stack is then associated with
 * the newThread when the signal handler associateStack is executed.
 */
void setUpStackTransfer() {
	setUpAction.sa_handler = (void *) associateStack;
	setUpAction.sa_flags = SA_ONSTACK;
	sigaction(SIGUSR1, &setUpAction, NULL);
}

/*
 *  Sets up the new thread.
 *  The startFunc is the function called when the thread starts running.
 *  It also allocates space for the thread's stack.
 *  This stack will be the stack used by the SIGUSR1 signal handler.
 */
Thread createThread(void (startFunc)()) {
	static int nextTID = 0;
	Thread thread;
	stack_t threadStack;

	if ((thread = malloc(sizeof(struct thread))) == NULL) {
		perror("allocating thread");
		exit(EXIT_FAILURE);
	}
	thread->tid = nextTID++;
	thread->state = SETUP;
	thread->start = startFunc;
	if ((threadStack.ss_sp = malloc(SIGSTKSZ)) == NULL) { // space for the stack
		perror("allocating stack");
		exit(EXIT_FAILURE);
	}
	// switch to next thread
	if (currentThread) {
		thread->prev = currentThread;
		//thread->next = currentThread;
		currentThread->next = thread;
		//currentThread->prev = thread;
		//thread->prev->next = thread;
	} else { // no existing currentThread
		currentThread = thread;
		currentThread->next = thread;
		currentThread->prev = thread;
	}


	thread->stackAddr = threadStack.ss_sp;
	threadStack.ss_size = SIGSTKSZ; // the size of the stack
	threadStack.ss_flags = 0;
	if (sigaltstack(&threadStack, NULL) < 0) { // signal handled on threadStack
		perror("sigaltstack");
		exit(EXIT_FAILURE);
	}
	orderedThreadList[numThreads++] = thread;
	newThread = thread; // So that the signal handler can find this thread
	currentThread = thread;
	kill(getpid(), SIGUSR1); // Send the signal. After this everything is set.
	return thread;
}


void setUpTimer() {
	timerAction.sa_handler = scheduler;
	sigaction(SIGVTALRM, &timerAction, NULL);

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 20000;	// 20 milliseconds
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 20000;

	if (setitimer(ITIMER_VIRTUAL, &timer, NULL) < 0) {
		perror("setitimer");
		exit(EXIT_FAILURE);
	}
}


int main(void) {
	struct thread controller;
	Thread threads[NUMTHREADS];
	mainThread = &controller;
	mainThread->state = RUNNING;
	setUpStackTransfer();
	Thread t1;
	Thread lastt;
	// create the threads
	for (int t = 0; t < NUMTHREADS; t++) {
		if (t == 0){
			t1 = createThread(threadFuncs[t]);
			lastt = t1;
		}else
			lastt = createThread(threadFuncs[t]);
	}
	setUpTimer();
	t1->prev=lastt;
	lastt->next = t1;
	printThreadStates();
	printf("%d -> %d\n",lastt->tid,lastt->next->tid);
	puts("switching to first thread ");
	switcher(mainThread, t1);
	puts("back to the main thread");
	printThreadStates();
	return EXIT_SUCCESS;
}
