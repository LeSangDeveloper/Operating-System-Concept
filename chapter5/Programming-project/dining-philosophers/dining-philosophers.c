#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_PHILOSOPHER 5
#define LEFT(n) ((n + NUM_PHILOSOPHER - 1) % NUM_PHILOSOPHER)
#define RIGHT(n) ((n + 1) % NUM_PHILOSOPHER)

pthread_mutex_t mutex;
pthread_cond_t cond_var[NUM_PHILOSOPHER];
int *philosophers;

enum {THINKING, HUNGRY, EATING} philosophers_states;

void pickup_forks(int philosopher_number);
void return_forks(int philosopher_number);
void think(int philosopher_num);
void check(int philosopher_num);
void eat(int philosopher_num);

void* philosopher_do(void *param);

int main() {
	printf("\n----------Starting the Philosopher Dining program----------\n");
	pthread_t threads[NUM_PHILOSOPHER];
	
	philosophers = malloc(5 * sizeof(int));
	pthread_mutex_init(&mutex, NULL);
	

	printf("\nInitializing conditions.......\n");
	for (int i = 0; i < NUM_PHILOSOPHER; ++i)
	{
		pthread_cond_init(&cond_var[i], NULL);
		philosophers[i] = THINKING;
	}

	printf("\nInitializing threads.......\n");
	for (int i = 0; i < NUM_PHILOSOPHER; ++i) {
		int *temp = malloc(sizeof(*temp));
		*temp = i;
		pthread_create(&threads[i], NULL, philosopher_do, (void*)temp);
	}

	for (int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_join(threads[i], NULL);
	}
	
	printf("\n----------End the Philosopher Dining program-----------\n");

	return 0;
}

void think(int phisolopher_num) {
	int thinking_time = (rand() % 5) + 3;
	printf("\nPhisolopher %d thinking for %d seconds.\n", phisolopher_num, thinking_time);
	sleep(thinking_time);
}

void eat(int philosopher_num) {
	pickup_forks(philosopher_num);

	int eating_time = (rand() % 5) + 3;
	printf("\nPhilosopher %d eating %d seconds.\n", philosopher_num, eating_time);
	sleep(eating_time);

	return_forks(philosopher_num);
	printf("\nPhisolopher %d finished eating.\n", philosopher_num);
}

void* philosopher_do(void *param) {
	int philosopher_number = *((int *)param);

	printf("\nPhilosopher %d starting...\n", philosopher_number);	
	think(philosopher_number);
	printf("\nPhilosopher %d is hungry...\n", philosopher_number);
	philosophers[philosopher_number] = HUNGRY; 
	eat(philosopher_number);

	return param;
}

void check(int philosopher_num) {
	if (philosophers[philosopher_num] == HUNGRY && philosophers[LEFT(philosopher_num)] != EATING && philosophers[RIGHT(philosopher_num)] != EATING) {
		philosophers[philosopher_num] = EATING;
		pthread_cond_signal(&cond_var[philosopher_num]);
	}
}

void pickup_forks(int philosopher_number) {
	pthread_mutex_lock(&mutex);

	check(philosopher_number);
	if (philosophers[philosopher_number] != EATING) {
		printf("\nPhilosopher %d is waiting....\n", philosopher_number);
		pthread_cond_wait(&cond_var[philosopher_number], &mutex);
		printf("\nPhilosopher %d finished waiting and start eating.\n", philosopher_number);
	}

	pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopher_number) {
	pthread_mutex_lock(&mutex);

	philosophers[philosopher_number] = THINKING;
	check(LEFT(philosopher_number));
	check(RIGHT(philosopher_number));

	pthread_mutex_unlock(&mutex);
}
