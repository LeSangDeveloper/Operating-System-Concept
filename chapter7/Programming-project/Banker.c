#include<stdio.h>
#include<pthread.h>
#include<time.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int safe_state[NUMBER_OF_CUSTOMERS];

pthread_mutex_t mtx;

int init_safe_state();
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);

void* customer_f(void* param);

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: ./banker [int] [int] [int].\n");
		return 1;
	}	

	printf("\n-----------START BANKER PROGRAM-----------\n");

	time_t t;
	srand((unsigned) time(&t));

	printf("\nInit available matrix.........");
	for (int i = 1; i < argc; ++i) {
		int temp = atoi(argv[i]);
		available[i - 1] = temp;
	}

	printf("\nInit maximum matrix.........");
	for (int j = 0; j < NUMBER_OF_CUSTOMERS; ++j)
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
		int temp = rand() % available[i];
		maximum[j][i] = temp;
	}

	printf("\nInit allocation matrix........");
	for (int j = 0; j < NUMBER_OF_CUSTOMERS; ++j) {
		for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
			int temp = rand() % available[i];
			while (temp > maximum[j][i]) temp = rand() % available[i];
			allocation[j][i] = temp;
		}
	}

	printf("\nInit need matrix.........");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
		for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) 
			need[i][j] = maximum[i][j] - allocation[i][j];

	printf("\nInit safe state...........");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
		safe_state[i] = -1;

	if (init_safe_state() != 0) {
		printf("\nCannot init safe state.\n");
		return 1;
	}

	printf("\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS - 1; ++i) {
		printf("P%d -> ", safe_state[i]);
	}
	printf("P%d", safe_state[NUMBER_OF_CUSTOMERS - 1]);

	pthread_mutex_init(&mtx, NULL);
	pthread_t customer_threads[NUMBER_OF_CUSTOMERS];

	return 0;
}

int init_safe_state() {
	int f[NUMBER_OF_CUSTOMERS];
	int temp = 0;
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
		f[i] = 0;

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
		for (int j = 0; j < NUMBER_OF_CUSTOMERS; ++j){
			if (f[j] == 0) {
				int flag = 0;
				for (int k = 0; k < NUMBER_OF_RESOURCES; ++k) {
					if (need[j][k] > available[k]) {
						flag = 1;
						break;
					}
				}
			
				if (flag == 0) {
					safe_state[temp++] = j;
					for (int k = 0; k < NUMBER_OF_RESOURCES; ++k) 
						available[k] += allocation[j][k];
					f[j] = 1;
				}				
				
			}
		}
	}

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
		if (safe_state[i] == -1) {
			return 1;
		}
	}

	return 0;
}
