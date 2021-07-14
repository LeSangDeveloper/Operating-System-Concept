#include<pthread.h>
#include<stdio.h>
#include<limits.h>

int arr[20]; /* this data is shared by the thread(s) */
int minValue;
int maxValue;
int averageValue;
int array_length;

void *max(void *param);
void *average(void *param);
void *min(void *param);

int main(int argc, char *argv[]) {
	pthread_t max_tid;
	pthread_t average_tid;
	pthread_t min_tid;
	pthread_attr_t attr;

	array_length = argc - 1;

	if (argc < 2) {
		fprintf(stderr, "usage: min_max_threads <array of int>\n");\
		return -1;
	}

	for (int i = 0; i < array_length; ++i) {
		arr[i] = atoi(argv[i+1]);
	}


	/*get the default attributes*/
	pthread_attr_init(&attr);
	/*create the thread*/
	pthread_create(&max_tid, &attr, max, argv[1]);
	pthread_create(&min_tid, &attr, min, argv[1]);
	/*wait for the thread to exit*/
	pthread_join(max_tid, NULL);	
	pthread_join(min_tid, NULL);	

	pthread_create(&average_tid, &attr, average, argv[1]);
	pthread_join(average_tid, NULL);

	printf("max = %d\n", maxValue);
	printf("min = %d\n", minValue);
	printf("average = %d\n", averageValue);

	return 0;
}

void *max(void *param) {
	maxValue = 0;

	for (int i = 0; i < array_length; ++i) {
		if (maxValue < arr[i])
			maxValue = arr[i];
	}

	pthread_exit(0);
}

void *average(void *param) {
	averageValue = (minValue + maxValue) / 2;

	pthread_exit(0);
}

void *min(void *param) {
	minValue = INT_MAX;

	for (int i = 0; i < array_length; ++i) {
		if (minValue > arr[i])
			minValue = arr[i];
	}

	pthread_exit(0);
}
