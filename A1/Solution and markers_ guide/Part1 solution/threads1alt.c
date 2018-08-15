/*
	The threads.
*/

void thread1() {
	int i;
	for (i = 0; i < 5; i++)
		puts("hi 1");
}

void thread2() { 
	int i;
	for (i = 0; i < 10; i++)
		puts("hi 2");
}

void thread3() {
	int i;
	for (i = 0; i < 5; i++)
		puts("hi 3");
}

void thread4() { 
	int i;
	for (i = 0; i < 10; i++)
		puts("hi 4");
}

void thread5() {
	int i;
	for (i = 0; i < 5; i++)
		puts("hi 5");
}

const int NUMTHREADS = 5;

typedef void (*threadPtr)();

threadPtr threadFuncs[] = {thread1, thread2, thread3, thread4, thread5};
