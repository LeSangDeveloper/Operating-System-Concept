#include<stdio.h>
#include<time.h>
#include<limits.h>

#define CYLINDER_MAX 4999
#define CYLINDER_MIN 0
#define CYLINDER_REQUEST_AMOUNT 1000

void copy_array(int a[], int b[], int n);
int FCFS(int cylinder_requests[], int n, int current_position);
int SSTF(int cylinder_requests[], int n, int current_posotion);
int SCAN(int cylinder_requests[], int n, int current_position);
int C_SCAN(int cylinder_requests[], int n, int current_position);
int LOOK(int cylinder_requests[], int n, int current_position);
int C_LOOK(int cylinder_requests[], int n, int current_position);

int main(int argc, char *argv[]) {

	if (argc != 2 || atoi(argv[1]) < CYLINDER_MIN || atoi(argv[1]) > CYLINDER_MAX) {
		printf("\nUSAGE: ./disk-shceduling <current position (0 -> 4999)>");
		return 1;
	}

	printf("\n----------BEGINING OF PROGRAM DISK SCHEDULING----------\n");

	int cylinder_requests[CYLINDER_REQUEST_AMOUNT];
	srand(time(0));

	for (int i = 0; i < CYLINDER_REQUEST_AMOUNT; ++i) {
		cylinder_requests[i] = rand();

		while (cylinder_requests[i] < CYLINDER_MIN || cylinder_requests[i] > CYLINDER_MAX) {
			cylinder_requests[i] = rand();
		}
	}

	printf("\nFCFS: %d", FCFS(cylinder_requests, CYLINDER_REQUEST_AMOUNT, atoi(argv[1])));
	printf("\nSSTF: %d", SSTF(cylinder_requests, CYLINDER_REQUEST_AMOUNT, atoi(argv[1])));
	printf("\nSCAN: %d", SCAN(cylinder_requests, CYLINDER_REQUEST_AMOUNT, atoi(argv[1])));
	printf("\nLOOK: %d", LOOK(cylinder_requests, CYLINDER_REQUEST_AMOUNT, atoi(argv[1])));
	printf("\nC_SCAN: %d", C_SCAN(cylinder_requests, CYLINDER_REQUEST_AMOUNT, atoi(argv[1])));
	printf("\nC_LOOK: %d", C_LOOK(cylinder_requests, CYLINDER_REQUEST_AMOUNT, atoi(argv[1])));

	printf("\n----------END OF PROGRAM-----------\n");

	return 0;
}

int FCFS(int cylinder_requests[], int n, int current_position) {
	int head_movement = 0;
	
	for (int i = 0; i < n; ++i) {
		int temp = cylinder_requests[i];

		if (temp > current_position) {
			head_movement += (temp - current_position);
		} else if (temp < current_position) {
			head_movement += (current_position - temp);
		}
		
		current_position = cylinder_requests[i];
	}

	return head_movement;
}

void copy(int a[], int b[], int n) {
	for (int i = 0; i < n; ++i) {
		b[i] = a[i];
	}
}

int SSTF(int cylinder_requests[], int n, int current_position) {
	int head_movement = 0;
	int temp[n];
	copy(cylinder_requests, temp, n);

	for (int i = 0; i < n; ++i) {
		int distance = INT_MAX;
		int temp_index = -1;
		for (int j = i; j < n; ++j) {
			int temp_distance = (current_position < temp[j]) ? (temp[j] - current_position) : (current_position - temp[j]);
			
			if (temp_distance < distance) {
				int temp_index = j;
				distance = temp_distance;
			}
		}
		
		if (i != temp_index) {
			int a = temp[i];
			temp[i] = temp[temp_index];
			temp[temp_index] = a;
		}

		current_position = temp[i];
		head_movement += distance;
	}

	return head_movement;
}

int SCAN(int cylinder_requests[], int n, int current_position) {
	return LOOK_SCAN(cylinder_requests, n, current_position, 0);
}

int LOOK(int cylinder_requests[], int n, int current_position) {
	int min = INT_MAX;
	for (int i = 0; i < n; ++i) {
		if (min > cylinder_requests[i]) min = cylinder_requests[i];
	}

	return LOOK_SCAN(cylinder_requests, n, current_position, min);
}

int LOOK_SCAN(int cylinder_requests[], int n, int current_position, int min) {
	int head_movement = 0;
	int left[n];
	int right[n];

	int left_length = 0;
	int right_length = 0;

	int left_index = 0;
	int right_index = 0;

	for (int i = 0; i < n; ++i) {
		if (cylinder_requests[i] < current_position) {
			left[left_index++] = cylinder_requests[i];
		} else {
			right[right_index++] = cylinder_requests[i];
		}
	}

	left[left_index++] = min;

	int temp_left = SSTF(left, left_index, current_position);
	int temp_right = SSTF(right, right_index, min);

	return temp_left + temp_right;
}

int C_SCAN(int cylinder_requests[], int n, int current_position) {
	return C_LOOK_SCAN(cylinder_requests, n, current_position, 0, 4999);
}

int C_LOOK(int cylinder_requests[], int n, int current_position) {
	int min = INT_MAX;
	int max = -1;
	for (int i = 0; i < n; ++i) {
		if (min > cylinder_requests[i]) min = cylinder_requests[i];
		if (max < cylinder_requests[i]) max = cylinder_requests[i];
	}

	return C_LOOK_SCAN(cylinder_requests, n, current_position, min, max);
}

int C_LOOK_SCAN(int cylinder_requests[], int n, int current_position, int min, int max) {
	int head_movement = 0;
	int left[n];
	int right[n];

	int left_length = 0;
	int right_length = 0;

	int left_index = 0;
	int right_index = 0;

	for (int i = 0; i < n; ++i) {
		if (cylinder_requests[i] < current_position) {
			left[left_index++] = cylinder_requests[i];
		} else {
			right[right_index++] = cylinder_requests[i];
		}
	}

	left[left_index++] = min;

	int temp_left = SSTF(left, left_index, current_position);
	int temp_right = SSTF(right, right_index, max);

	return temp_left + temp_right + 4999;
}

