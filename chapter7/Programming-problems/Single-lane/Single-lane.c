#include<stdio.h>
#include<pthread.h>

#define MAX_FARMERS 5

pthread_mutex_t mutex;

enum {SOUTH, NORTH} DIRECTION;
int direct = NORTH;

void* north_f(void* param);
void* south_f(void* param);
//void* manager_f(void* param);
 void* manager_f(void *param) {
 	while (1) {
 		sleep(2);
 		pthread_mutex_lock(&mutex);

 		if (direct == SOUTH) direct = NORTH;
 		else direct = SOUTH;

 		pthread_mutex_unlock(&mutex);
 	}

}

int main() {
	pthread_mutex_init(&mutex, NULL);
	int test = 1;
	printf("------START BRIDGE LANE PROGRAM -----");
	pthread_t north_threads[5],  south_threads[5], manager_thread;
	
    pthread_create(&manager_thread, NULL, manager_f, "thread 1");

	for (int i = 0; i < MAX_FARMERS; ++i) {
		pthread_create(&north_threads[i], NULL, north_f, &i);
		pthread_create(&south_threads[i], NULL, south_f, &i);
	}

	for (int i = 0; i < MAX_FARMERS; ++i) {
		pthread_join(south_threads[i], NULL);
		pthread_join(north_threads[i], NULL);
	}

//	pthread_join(manager_thread, NULL);
	return 0;
}

void* north_f(void* param) {
	int waiting = 1;

	while(waiting == 1) {
		sleep(1);
		pthread_mutex_lock(&mutex);
		if (direct == NORTH) waiting = 0;
		pthread_mutex_unlock(&mutex);
	}

	printf("\nNorth farmer go through bridge.");

}

void* south_f(void* param) {

	int waiting = 1;
	while (waiting == 1) {
		sleep(1);
		pthread_mutex_lock(&mutex);
		if (direct == SOUTH) waiting = 0;
		pthread_mutex_unlock(&mutex);
	}

	printf("\nSorth Farmer go through bridge.");

}

