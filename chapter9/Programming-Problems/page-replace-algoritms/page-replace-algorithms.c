#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define PAGE_REF_STRING_LENGTH 10
#define PAGE_FRAMES 3
#define EMPTY_FRAME -2

int page_string[PAGE_REF_STRING_LENGTH];

void enqueue(int queue[], int final_idx, int value) {
	
	if (final_idx == 0) {
		queue[0] = value;
	} else {
		for (int i = final_idx; i > 0; --i) {
			queue[i] = queue[i - 1];
		}
		queue[0] = value;
	}
}

int find_in_array(int array[], int n, int value) {
	for (int i = 0; i < n; ++i) {
		if (array[i] == value) {
			return i;
		}
	}

	return -1;
}

int update_queue(int queue[], int n, int value) {
	int index = find_in_array(queue, n, value);
	int temp;
	if (index != -1) {
		temp = value;
		enqueue(queue, index, value);	
	} else {
		temp = queue[n - 1];
		enqueue(queue, n - 1, value);
	}

	return temp;
}

int is_full(int frames[], int n) {
	for (int i = 0; i < n; ++i) {
		if (frames[i] == EMPTY_FRAME) {
			return 0;
		}
	}
	return 1;
}

void print_frames(int frames[], int n) {
	printf("\nFRAMES: ");
	for (int i = 0; i < n; ++i) {
		if (frames[i] == EMPTY_FRAME) {
			printf(" EMPTY ");
		} else {
			printf(" %d ", frames[i]);
		}
	}

	printf("\n");
}

int LRU() {
	printf("\n-----------USING LRU-------------\n");
	int frames[PAGE_FRAMES];
	int queue[PAGE_FRAMES];
	int page_fault = 0;

	for (int i = 0; i < PAGE_FRAMES; ++i) {
		frames[i] = EMPTY_FRAME;
	}

	for (int i = 0; i < PAGE_REF_STRING_LENGTH; ++i) {

		int value = page_string[i];

		printf("\n----value=%d-----\n", value);

		if (is_full(frames, PAGE_FRAMES) == 1) {
			int temp = update_queue(queue, PAGE_FRAMES, value);
			if (temp != value) {
				int temp_idx = find_in_array(frames, PAGE_FRAMES, temp);
				frames[temp_idx] = value;
				page_fault++;
			}
		} else {
			int idx = find_in_array(frames, PAGE_FRAMES, value);

			if (idx == -1) {
				printf("\nADD PAGE\n");
				int empty_idx = find_in_array(frames, PAGE_FRAMES, EMPTY_FRAME);
				frames[empty_idx] = value;
				update_queue(queue, PAGE_FRAMES, value);
			}

		}

		print_frames(frames, PAGE_FRAMES);
	}

	return page_fault;

}

int FIFO() {
	int frames[PAGE_FRAMES];
	int oldest_idx = 0;
	int page_fault = 0;

	for (int i = 0; i < PAGE_FRAMES; ++i) {
		frames[i] = EMPTY_FRAME;
	}

	printf("\n------------USING FIFO-----------\n");

	for (int i = 0; i < PAGE_REF_STRING_LENGTH; ++i) {
		int value = page_string[i];

		printf("\n----value=%d-----\n", value);

		if (is_full(frames, PAGE_FRAMES) == 1) {
			int idx = find_in_array(frames, PAGE_FRAMES, value);

			if (idx == -1) {
				frames[oldest_idx] = value;
				oldest_idx = (oldest_idx + 1) % PAGE_FRAMES;
				page_fault++;
			}

		} else {
			int temp_idx = find_in_array(frames, PAGE_FRAMES, value);
			if (temp_idx == -1) {
				int empty_idx = find_in_array(frames, PAGE_FRAMES, EMPTY_FRAME);
				frames[empty_idx] = value;
			}
		}

		print_frames(frames, PAGE_FRAMES);
	}

	return page_fault;

}

int OPT() {
	int frames[PAGE_FRAMES];
	int page_fault = 0;

	for (int i = 0; i < PAGE_FRAMES; ++i) {
		frames[i] = EMPTY_FRAME;
	}

	printf("\n------------USING OPT-----------\n");

	for(int i = 0; i < PAGE_REF_STRING_LENGTH; ++i) {
	
		int value = page_string[i];

		printf("\n-----value=%d-----\n", value);

		if (is_full(frames, PAGE_FRAMES) == 1) {
			int idx = find_in_array(frames, PAGE_FRAMES, value);

			if (idx == -1) {
				int temp_idx = -1;
				int furthest_frame = EMPTY_FRAME;
				int furthest_idx = -1;
				for (int j = 0; j < PAGE_FRAMES; ++j) {
					int temp_frame = frames[j];
				       	int flag = 1;	
					for (int k = i; k < PAGE_REF_STRING_LENGTH; ++k) {
						if (page_string[k] == temp_frame) {
							if (furthest_idx < k) {
								furthest_idx = k;
								furthest_frame = temp_frame;
								temp_idx = find_in_array(frames, PAGE_FRAMES, furthest_frame);
							}
							flag = 0;
							break;
						}
					}

					if (flag == 1) {
						temp_idx = find_in_array(frames, PAGE_FRAMES, temp_frame);
						break;	
					}
				}

				frames[temp_idx] = value;
				page_fault++;
			} 

		} else {
			int temp_idx = find_in_array(frames, PAGE_FRAMES, value);
			if (temp_idx == -1) {
				int empty_idx = find_in_array(frames, PAGE_FRAMES, EMPTY_FRAME);
				frames[empty_idx] = value;
			}
		}

		print_frames(frames, PAGE_FRAMES);
	}
	
	return page_fault;

}

int main() {
	srand(time(0));

	printf("\nPage string: ");

	for(int i = 0; i < PAGE_REF_STRING_LENGTH; ++i) {
		page_string[i] = rand() % 10;
		printf(" %d ", page_string[i]);
	}

	int fifo = FIFO();
	int opt = OPT();
	int lru = LRU(); 
        
 	printf("\n---------SUMMARY--------\n");	
	printf("\nFIFO: %d", fifo);
	printf("\nOPT: %d", opt);
	printf("\nLRU: %d\n", lru);

	return 0;
}
