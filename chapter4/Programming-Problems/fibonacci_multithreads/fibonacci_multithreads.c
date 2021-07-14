#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int arr[1000];
int fibonacciLength;
int fibonacciFinish = 0;

void *fibonacci(void *param);
void *output(void *param);

int main(int argc, char *argv[]) {
	pthread_t fibonacci_tid; /*fibonacci thread tid*/	
	pthread_t output_tid; /*output thread tid*/
	pthread_attr_t attr;

	if (argc != 2) {
		fprintf(stderr, "usage: fibonaci_multithreads <Integer value>\n");
		return -1;
	}

	if (atoi(argv[1]) < 0) {
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}

	pthread_attr_init(&attr);
	pthread_create(&fibonacci_tid, &attr, fibonacci, argv[1]);
	pthread_create(&output_tid, &attr, output, &fibonacci_tid);

	pthread_join(fibonacci_tid, NULL);
	pthread_join(output_tid, NULL);
	return 0;
}

void *output(void *param) {
	printf("Waiting fibonacci thread...\n");

	while (fibonacciFinish == 0);
	
	for (int i = 0; i < fibonacciLength; ++i) {
		printf("%d ", arr[i]);
	}

	printf("\nFinish waiting\n");
	
	pthread_exit(0);
}

void *fibonacci(void *param) {
	int upper = atoi(param);
	fibonacciLength = 2;
	arr[0] = 0;
	arr[1] = 1;

	for (int i = 2; i < upper; i++) {
		arr[i] = arr[i - 1] + arr[i - 2];
		fibonacciLength++;
	}

	fibonacciFinish = 1;

	pthread_exit(0);
}
