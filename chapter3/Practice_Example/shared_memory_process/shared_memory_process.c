#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>

int global = 5;

int main() {
	int local = 10;

	pid_t pid = fork();

	if (pid == 0) { /*child process*/
		global += 15;
		local += 15;
		printf("Child: global = %d, local = %d", global, local);
		return 0;
	}	
	else if (pid > 0) { /*parent process*/
		wait(NULL);
		printf("PARENT: global = %d, local = %d", global, local);
		return 0;
	}
}
