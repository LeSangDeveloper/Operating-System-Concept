#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main() {
	pid_t pid;
	
	/* fork a child process */
	pid = fork();
	
	if (pid < 0) {
		fprintf(stderr, "Fork Failed");
	}
	else if (pid == 0) {
		execlp("/bin/ls", "ls", NULL);
		printf("\nChild Complete");
		exit(0);
	}
	else {
		printf("\nParent wait....");
		wait(NULL);
		printf("\nComplete\n");
	}

	return 0;	
}
