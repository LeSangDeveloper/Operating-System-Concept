#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include"buffer.h"

int START_NUMBER = 1;
buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int inserted_pointer;
int removed_pointer;

int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer_do(void *param);
void *consumer_do(void *param);

int main(int argc, char *argv[]) {
	/*
	 * 1. Get command line arguments argv[1], argv[2], argv[3]
	 * 2. Initiaize buffer
	 * 3. Create producer thread(s)
	 * 4. Create consumer thread(s)
	 * 5. sleep
	 * 6. Exit
	 * */
	int sleep_time, producer_threads, consumer_threads;
	pthread_t pid, cid;

	if (argc != 4) {
		fprintf(stderr, "Usage: <sleeptime> <producer's quantity> <consumer's quantity>");
		return -1;
	}

	sleep_time = atoi(argv[1]);
	producer_threads = atoi(argv[2]);
	consumer_threads = atoi(argv[3]);

	inserted_pointer = 0;
	removed_pointer = 0;
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);

	for (int i = 0; i < producer_threads; i++) {
		pthread_create(&pid, NULL, &producer_do, NULL);
	}

	for (int j = 0; j < consumer_threads; ++j) {
		pthread_create(&cid, NULL, &consumer_do, NULL);
	}

	sleep(sleep_time);

	return 0;
}

int insert_item(buffer_item item) {
	/* insert item into buffer
	 * return 0 if successful, otherwise
	 * return -1 indicating an error condition
	 * */
	buffer[inserted_pointer] = item;
	inserted_pointer = (inserted_pointer + 1) % BUFFER_SIZE;

	return 0;
}

int remove_item(buffer_item *item) {
	/*
	 * remove an object from buffer
	 * placing it in item
	 * return 0 if successful, otherwise
	 * return -1 indicating an error condition
	 * */
	*item = buffer[removed_pointer];
	removed_pointer = (removed_pointer + BUFFER_SIZE - 1) % BUFFER_SIZE; 

	return 0;
}

void *consumer_do(void *param) {
	/*do {
	 * ...
	 * remove an item from buffder to next_consumed
	 * ...
	 * signal(mutex);
	 * signal(empty);
	 * ....
	 * consumed the item in next-consumed
	 * ...
	 * } while (true)
	 * */

	buffer_item item;

	while (1) {
		sleep(2);
		sem_wait(&empty);
		pthread_mutex_lock(&mutex); 
			
		remove_item(&item);
		printf("\nConsumer %u consumed %d \n", (unsigned int)pthread_self(), item);

		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}

	return param;
}

void *producer_do(void *param) {
	
	buffer_item item;

	while(1) {
		sleep(2);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);


		item = START_NUMBER++;
		insert_item(item);

		printf("Producer %u produced %d \n", (unsigned int)pthread_self(), item);

		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}

	return param;
}
