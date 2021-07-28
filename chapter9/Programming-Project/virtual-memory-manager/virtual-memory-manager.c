#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<assert.h>
#include<unistd.h>

#define OFFSET_BITS 8
#define PAGE_BITS 8
#define VIRTUAL_MEMORY_BITS 16

#define PHYSICAL_MEMORY_FRAMES 256
#define PAGE_TABLE_FRAMES 256
#define PAGE_SIZE 256
#define PHYSICAL_MEMORY_SIZE PHYSICAL_MEMORY_FRAMES * PAGE_SIZE
#define PAGE_NUMBER_MASK 65280
#define OFFSET_MASK 255
#define TLB_SIZE 16

#define NOT_FOUND -1
#define EMPTY -1

struct TLB_table {
	unsigned int page_number;
	unsigned int frame_number;
};

int find_in_array(struct TLB_table array[], int n, int value);
void enqueue_tlb(struct TLB_table queue[], int *n, int page_number, int frame_number);

int main(int argc, char *argv[]) {
	printf("\n----------VIRTUAL MEMOMRY MANAGER-----------\n");

	if (argc != 2) {
		printf("\nUSAGE: ./virtual-memory-manager <filename>\n");
		return 1;
	}

	FILE *addresses = fopen(argv[1], "r");
	FILE *backing_store = fopen("BACKING_STORE.bin", "rb");
	FILE *output =fopen("output.txt", "w");

	int physical_memory[PHYSICAL_MEMORY_SIZE];
	char buffer[256];

	int logical_address = 0;
	int frame = 0;
	int tlb_size = 0;

	int tlb_hit_count = 0;
	int address_count = 0;
	int page_fault_count = 0;

	struct TLB_table tlb_queue[TLB_SIZE];
	int page_table[PAGE_TABLE_FRAMES];

	memset(tlb_queue, EMPTY, TLB_SIZE*sizeof(char));
	memset(page_table, EMPTY, PAGE_TABLE_FRAMES*sizeof(int));

	while(fscanf(addresses, "%d", &logical_address) == 1) {
		address_count++;
		int page_number = (logical_address & PAGE_NUMBER_MASK) >> (VIRTUAL_MEMORY_BITS - OFFSET_BITS);
		int page_offset = logical_address & OFFSET_MASK;
	
		int idx = find_in_array(tlb_queue, tlb_size, page_number);	

		int hit = (idx == NOT_FOUND) ? -1 : tlb_queue[idx].frame_number;
		int physical_address = hit * PAGE_SIZE + page_offset;

		int value = 0;

		if (hit != -1) {
			tlb_hit_count++;
		} else if (page_table[page_number] == EMPTY) {
			page_fault_count++;

			fseek(backing_store, page_number*PAGE_SIZE, SEEK_SET);
			fread(buffer, sizeof(char), PAGE_SIZE, backing_store);

			page_table[page_number] = frame;

			int begin_of_frame_in_physical = frame * PAGE_SIZE;

			for (int i = 0; i < PAGE_SIZE; ++i) {
				physical_memory[begin_of_frame_in_physical + i] = buffer[i];
			}

			frame++;

			enqueue_tlb(tlb_queue, &tlb_size, page_number, page_table[page_number]);

			physical_address = begin_of_frame_in_physical + page_offset;
		} else {
			physical_address = page_table[page_number] * 256 + page_offset;
		}

		value = physical_memory[physical_address];
		fprintf(output, "\nVirtual address: %d, Physical address: %d, Value: %d", logical_address, physical_address, value);
	}
	
	fclose(addresses);
	fclose(backing_store);

	fprintf(output, "\nNumber of Addresses: %d", address_count);
	fprintf(output, "\nNumber of Page faults: %d", page_fault_count);
	fprintf(output, "\nPage fault rate: %f", page_fault_count*1.0f / address_count);
	fprintf(output, "\nTLB Hits: %d", tlb_hit_count);
	fprintf(output, "\nTLB hit rate: %f\n", tlb_hit_count * 1.0f / address_count);

	fclose(output);

	printf("\n------------END PROGRAM-------------\n");

	return 0;
}

int find_in_array(struct TLB_table array[], int n, int value) {
	for (int i = 0; i < n; ++i) {
		if (array[i].page_number == value)
			return i;
	}

	return NOT_FOUND;
}

void enqueue_tlb(struct TLB_table queue[], int *n, int page_number, int frame_number) {
	if (*n > 0) {
		if (*n == TLB_SIZE) {
			*n = *n - 1;
		}

		for (int i = *n; i > 0; --i) {
			queue[i].page_number = queue[i - 1].page_number;
			queue[i].frame_number = queue[i - 1].frame_number;
		}

	} 

	if (*n <= 15) *n = *n + 1;

	queue[0].page_number = page_number;
	queue[0].frame_number = frame_number;
}
