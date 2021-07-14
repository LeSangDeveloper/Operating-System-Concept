// C Program to implement merge sort using
// multi-threading
#include <stdio.h>
#include <pthread.h>
#include <time.h>

// number of elements in array
#define MAX 20

// number of threads
#define THREAD_MAX 4

// array of size MAX
int a[MAX];
int part = 0;

// merge function for merging two parts
void merge_2_parts(int low, int mid, int high)
{
	int *left = (int *)malloc((mid - low + 1) * sizeof(int));
	int *right = (int *)malloc((high - mid) * sizeof(int));
	// n1 is size of left part and n2 is size
	// of right part
	int n1 = mid - low + 1, n2 = high - mid, i, j;

	// storing values in left part
	for (i = 0; i < n1; i++)
		left[i] = a[i + low];

	// storing values in right part
	for (i = 0; i < n2; i++)
		right[i] = a[i + mid + 1];

	int k = low;
	i = j = 0;

	// merge left and right in ascending order
	while (i < n1 && j < n2) {
		if (left[i] <= right[j])
			a[k++] = left[i++];
		else
			a[k++] = right[j++];
	}

	// insert remaining values from left
	while (i < n1) {
		a[k++] = left[i++];
	}

	// insert remaining values from right
	while (j < n2) {
		a[k++] = right[j++];
	}
}

// merge sort function
void merge_sort(int low, int high)
{
	// calculating mid point of array
	int mid = low + (high - low) / 2;
	if (low < high) {

		// calling first half
		merge_sort(low, mid);

		// calling second half
		merge_sort(mid + 1, high);

		// merging the two halves
		merge_2_parts(low, mid, high);
	}
}

// thread function for multi-threading
void* merge_sort_thread(void* arg)
{
	// which part out of 4 parts
	int thread_part = part++;

	// calculating low and high
	int low = thread_part * (MAX / 2);
	int high = (thread_part + 1) * (MAX / 2) - 1;

	// evaluating mid point
	int mid = low + (high - low) / 2;
	if (low < high) {
		merge_sort(low, mid);
		merge_sort(mid + 1, high);
		merge_2_parts(low, mid, high);
	}
}

// Driver Code
int main()
{
	// generating random values in array
	for (int i = 0; i < MAX; i++)
		a[i] = rand() % 100;

	// t1 and t2 for calculating time for
	// merge sort
	clock_t t1, t2;

	t1 = clock();
	pthread_t threads[THREAD_MAX];

	// creating 2 threads
	for (int i = 0; i < 2; i++)
		pthread_create(&threads[i], NULL, merge_sort_thread, (void*)NULL);

	// joining all 2 threads
	for (int i = 0; i < 2; i++)
		pthread_join(threads[i], NULL);

	merge_2_parts(0, (MAX - 1)/2, MAX - 1);	

	t2 = clock();

	// displaying sorted array
	printf("Sorted array: ");
	for (int i = 0; i < MAX; i++)
		printf("%d ", a[i]);

	// time taken by merge sort in seconds
	printf("\nTime taken: %.4f\n", (t2 - t1) / (double)CLOCKS_PER_SEC);

	return 0;
}

