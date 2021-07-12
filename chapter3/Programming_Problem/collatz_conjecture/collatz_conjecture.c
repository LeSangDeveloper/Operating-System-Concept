#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
	#include<sys/mman.h>

const int SIZE = 4096;
const char *name = "COLLATZ";
const char *three_digit = "xxx, ";
const char *two_digit = "xx, ";
const char *one_digit = "x, ";

int cal_len(int num) {
	if (num >= 100) {
		return strlen(three_digit);
	} else if (num >= 10) {
		return strlen(two_digit);
	} else {
		return strlen(one_digit);
	}
}

void writeCollatzShm(int num) {
	int shm_collatz;
	void *ptr;
	char *c;
	shm_collatz = shm_open(name, O_CREAT | O_RDWR, 0666);

	ftruncate(shm_collatz, SIZE);

	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_collatz, 0);

	sprintf(ptr, "%d, ", num);
	ptr += cal_len(num);

	while (num > 1) {
		if (num % 2 == 0) {
			num = num / 2;
			if (num == 1) {	
				sprintf(ptr, "1\n"); 
				ptr += 1;
			}
			else {
				sprintf(ptr, "%d, ", num);
				ptr += cal_len(num);
			}
		} else {
			num = 3 * num + 1;
			sprintf(ptr, "%d, ", num);
			ptr += cal_len(num);
		}
	}
}

void readCollatzShm() {
	int shm_collatz;
	void *ptr;
	
	shm_collatz = shm_open(name, O_RDONLY, 0666);

	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_collatz, 0);
	
	printf("%s", (char *)ptr);
	shm_unlink(name);
}

int main(int argc, char* argv[]) {
	int num;
	pid_t pid;

	if (argc < 2) {
		printf("input error");
		return 1;
	}

	num = atoi(argv[1]);

	pid = fork();

	if (pid < 0) {
		printf("Fork error !");
		return 0;
	}

	if (pid == 0) { /*Child process*/
		writeCollatzShm(num);
		printf("Array: \n");
	} else { /*Parent process*/
		wait(NULL);
		readCollatzShm();
	}	

	return 0;
}
