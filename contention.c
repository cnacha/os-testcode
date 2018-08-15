#include <stdio.h>
#include <pthread.h>


int counter = 0;
int lock = 0;
pthread_mutex_t mutex;
int test_and_set(int *target){
	int rv = *target;
	*target = 1;
	return rv;
}

int compare_and_swap(int *value, int expected, int new_value){
	
	int temp = *value;
	if(*value == expected)
		*value = new_value;
	return temp;
}

void *increment_100(void *id) {
	// *****locking
	//while(test_and_set(&lock));
	//while(compare_and_swap(&lock,0,1));
	pthread_mutex_lock(&mutex);

	for (int i = 0; i < 10000000; i++) {
		counter++;
	}
	// **** unlocking
	lock = 0;
	pthread_mutex_unlock(&mutex);

	printf("thread: %ld counter: %d\n", (long)id, counter);
}

void main() {
	int const num_threads = 10;
	pthread_t thread_refs[num_threads];

	for (long i = 0; i < num_threads; i++) {
		pthread_create(&thread_refs[i], NULL, increment_100, (void *)i);
	}
	for (int i = 0; i < num_threads; i++) {
		pthread_join(thread_refs[i], NULL);
	}
}

