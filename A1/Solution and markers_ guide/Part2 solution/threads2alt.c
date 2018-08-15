/*
	The threads.
*/

void threadYield();

void thread1() {
	int i;
	for (i = 0; i < 2; i++) {
		puts("hi 1");
		threadYield();
	}
}

void thread2() { 
	int i;
	for (i = 0; i < 2; i++) {
		puts("hi 2");
		threadYield();
	}
}

void thread3() {
	int i;
	for (i = 0; i < 2; i++) {
		puts("hi 3");
		threadYield();
	}
}

void thread4() { 
	int i;
	for (i = 0; i < 5; i++) {
		puts("hi 4");
		threadYield();
	}
}

void thread5() {
	int i;
	for (i = 0; i < 2; i++) {
		puts("hi 5");
		threadYield();
	}
}

const int NUMTHREADS = 5;

typedef void (*threadPtr)();

threadPtr threadFuncs[] = {thread1, thread2, thread3, thread4, thread5};
