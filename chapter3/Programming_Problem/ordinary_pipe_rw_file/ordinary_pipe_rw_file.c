#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {
	int fd[2];
	pid_t pid;

	if (argc < 3) {
		fprintf(stderr, "Input error");
		return 1;
	}

	/*create a pipe*/
	if (pipe(fd) == -1) {
		fprintf(stderr, "Pipe failed");
		return 1;
	}

	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork Failed");
		return 1;
	}	

	if (pid > 0) { /*parent process*/
		/*close the unused end of the pipe*/
		close(fd[READ_END]);

		/*write to the pipe*/
//		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		FILE *fp;
		char readFromFile[BUFFER_SIZE];
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			fprintf(stderr, "error read file");
			return 1;
		}
		fscanf(fp, "%s", readFromFile);
		printf("write to pipe: %s", readFromFile);
		write(fd[WRITE_END], readFromFile, strlen(readFromFile)+1);

		/*close the write end of the pipe*/
		close(fd[WRITE_END]);
	} else { /*child process*/
		/*close the unused end of the pipe*/
		close(fd[WRITE_END]);

		/*read from the pipe*/
//		read(fd[READ_END], read_msg, BUFFER_SIZE);
//		printf("read %s", read_msg);
		char message[BUFFER_SIZE];
		FILE *fp;
		read(fd[READ_END], message, BUFFER_SIZE);
		
		fp = fopen(argv[2], "wb");
		if (fp == NULL) {
			printf("write file error!");
			return 1;
		}
		fprintf(fp, "%s", message);
		printf("\nRead from pipe: %s", message);
		/*close the write end of the pipe*/
		close(fd[READ_END]);
	}

	printf("\n");
	return 0;
}
