#include<stdio.h>
#include<unistd.h>

int main() {
	fork();

	fork();
	
	fork();
	
	fork();

	fork();

	wait(NULL);
	
	printf("%d\n", 1);
	return 0;
}
