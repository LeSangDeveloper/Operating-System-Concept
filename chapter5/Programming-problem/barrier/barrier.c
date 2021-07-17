#include<stdio.h>
#include<pthread.h>

#define NUM_THREADS 5

int barrier;

/*init the  barrier to the specified size*/
int init(int n);

/*Identifiers barrier point*/
int barrier_point(void); 

void *f(void *param);

int main() {
	printf("----------- Start program ------------------------");
	init(NUM_THREADS);

	pthread_t threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_create(&threads[i], NULL, f, NULL);
	}

	/* do some work for awhile */

//	barrier_point();

	/* do some work for awhile */
	
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	printf("\nAll threads jumped out\n-------------System finisded----------------\n");

	return 0;
}

void *f(void *param) {
	printf("\nInit thread");

	barrier_point();

	return param;
}

int init(int n) {
	barrier = n;

	return 0;
}

int barrier_point(void) {
	barrier--;
	printf("\nThreads in barrier: %d", 5 - barrier);
	
	while (barrier > 1);

	if(barrier < 0) return -1;
	
	return 0;
}
