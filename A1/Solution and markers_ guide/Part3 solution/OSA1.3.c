/*
 ============================================================================
 Name        : OSA1.3.c
 Author      : Robert Sheehan
 Version     : 22/07/2017
 Description : Simple thread library.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "littleThread.h"
#include "threads3.c"

Thread newThread; // the thread currently being set up
Thread mainThread; // the main thread
Thread currentThread = NULL; // the currently executing thread
Thread orderedThreadList[100]; // to print out the status
int numThreads = 0;	// the number of threads created

struct sigaction setUpAction;
struct sigaction timerAction;

struct itimerval timer;

/*
 * Converts the thread state to a string.
 */
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

/*
 *  Displays the states of all threads.
 */
void printThreadStates() {
	int i;
	puts("\nThread States\n=============");
	for (i = 0; i < numThreads; i++)
		printf("threadID: %d state:%s\n", orderedThreadList[i]->tid, threadState(
						orderedThreadList[i]->state));
	puts("");
}


/*
 * Switches execution from prevThread to nextThread.
 */
void switcher(Thread prevThread, Thread nextThread) {
	currentThread = nextThread;
	if (prevThread->state == FINISHED) {
		nextThread->state = RUNNING;
		printf("disposing %d\n", prevThread->tid);
		printThreadStates();
		free(prevThread->stackAddr); // just thrown away my own stack
		longjmp(nextThread->environment, 1);
	} else if (setjmp(prevThread->environment) == 0) { // so we can come back here
		prevThread->state = READY;
		nextThread->state = RUNNING;
		printThreadStates();
		longjmp(nextThread->environment, 1);
	}
}

/*
 * The very simple scheduler.
 * It cycles around the list of active threads.
 *
 */
void scheduler() {
	// puts("scheduler");
	if (currentThread == currentThread->next && currentThread->state == FINISHED) {
		printf("disposing %d\n", currentThread->tid);
		free(currentThread->stackAddr);	// this should really be protected as not async safe
		longjmp(mainThread->environment, 1); // escapes if no more threads
	}
	switcher(currentThread, currentThread->next);
}

/*
 * Removes a thread from execution.
 * At the moment only called when the thread finishes.
 */
void removeThread(Thread thread) {
	thread->state = FINISHED;
	thread->prev->next = thread->next;
	thread->next->prev = thread->prev;
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
	if (setjmp(localThread->environment) != 0) {
		(localThread->start)();
		removeThread(localThread);
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
 *  It also allocates space for the threads stack.
 *  This stack will be the stack used by the SIGUSR1 signal handler.
 */
Thread createThread(void( startFunc)()) {
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
	if (currentThread) {
		thread->prev = currentThread->prev;
		thread->next = currentThread;
		currentThread->prev = thread;
		thread->prev->next = thread;
	} else { // no existing currentThread
		currentThread = thread;
		currentThread->next = thread;
		currentThread->prev = thread;
	}
	if ((threadStack.ss_sp = malloc(SIGSTKSZ)) == NULL) { // space for the stack
		perror("allocating stack");
		exit(EXIT_FAILURE);
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
	kill(getpid(), SIGUSR1); // Send the signal. After this everything is set.

	threadStack.ss_flags = SS_DISABLE;
	if (sigaltstack(&threadStack, NULL) < 0) { // signal handled on threadStack
		perror("sigaltstack disabled");
		exit(EXIT_FAILURE);
	}	
	return thread;
}

/*
 * Sets up a timer to interrupt every 20 milliseconds
 * and call the scheduler.
 */
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
	Thread t1;
	mainThread = &controller;
	mainThread->state = RUNNING;
	setUpStackTransfer();
	// create the threads
	for (int t = 0; t < NUMTHREADS; t++) {
		if (t == 0)
			t1 = createThread(threadFuncs[t]);
		else
			createThread(threadFuncs[t]);
	}
	printThreadStates();

	setUpTimer();	// there is a race condition
					// if scheduler is called before a thread has been started
					// it will cause a crash

	puts("switching to first thread");
	switcher(mainThread, t1);
	puts("\nback to the main thread");
	printThreadStates();
	return EXIT_SUCCESS;
}
