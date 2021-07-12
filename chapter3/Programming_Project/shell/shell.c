
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<limits.h>

#define MAX_LINE 80 /*The maximum length command*/

char past_command[10][MAX_LINE];
int argCount = 0;

void addCommandToArr(char *command) {
	for (int i = 9; i > 0; --i) {
		strcpy(past_command[i], past_command[i - 1]);
	}

	strcpy(past_command[0], command);
	++argCount;
}

// Tokenization function: tokenizes the input stream and separates them on " "
int tokenize(char *string, char **ret) {	
	int i = 0;
	char *token[41];
	token[0] = strtok(string, " ");
	
	while(token[i] != NULL) {
		ret[i] = token[i];
		token[++i] = strtok(NULL, " ");
	}
	
	char arr1[10];
	strcpy(arr1, ret[i - 1]);
	token[i] = strtok(ret[i - 1], "&");
	
	if(strcmp(token[i], arr1) == 0) {
		
		ret[i] = NULL;
		return 0;
	}
	
	else {
		
		ret[i - 1] = token[i];
		ret[i] = NULL;
		return 1;
	}
}

int inputStringShell(char *input, int *has_ampersand) {
	char c;
	int conn = 1;
	int i = 0;
	while (conn == 1) {
		scanf("%c", &c);
		if (c == '\n') {
			conn = 0;
		} else if (c == '&'){
			input[i++] = ' ';
			*has_ampersand = 1;		
		} else {
			input[i++] = c;
		}
	}
	
	input[i++] = '\0';
	return i + 1;
}

void show_history() {
	if (argCount == 0) {
		printf("No command has been entered so far.\n");
	} else if(argCount < 11) {
		for (int i = 0; i < argCount; ++i)
			printf("%d\t%s\n", argCount - i, past_command[i]);
	} else {
		for (int i = 0; i < 10; ++i)
			printf("%d\t%s\n", argCount - i, past_command[i]);
	}
}

int main(void) {
	char *args[MAX_LINE/2 + 1];
	int should_run = 1; /*flag to determine when to exit program*/

	while (should_run) {
		printf("Sang's shell>");
		fflush(stdout);

		int has_ampersand = 0;
		char input[50];
		int length = inputStringShell(input, &has_ampersand);		
		int isSpecialCommand = 0;        	
		/**
		* After reading user input, the steps are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke exevcp()
		* (3) if the command included &, parent will invoke await
		*/	

		pid_t pid;
		pid = fork();

		tokenize(input, args);

		if (strcmp(input, "quit") == 0 || strcmp(input, "history") == 0) {
			isSpecialCommand = 1;
			if (pid == 0) return 0;
		}
	
		if (strcmp(input, "quit") == 0) {
			should_run = 0;
			return 0;
		}

		if (strcmp(input, "history") == 0 && pid != 0) {
			show_history();
		}

		if (input[0] == '!') {
			if (argCount == 0) {
				printf("No recent command hasn't been executed");	
			} else {
				if (input[1] == '!') {					
					char s1[MAX_LINE];
					strcpy(s1, past_command[0]);
					tokenize(s1, args);

					char command[MAX_LINE];
					strcpy(command, past_command[0]);
					tokenize(command, args);
				}
				else {
					int n = input[1] - 48;

					if (n >= 11 || n <= 0)
						printf("past command error");

					char s1[MAX_LINE];
					strcpy(s1, past_command[n - 1]);
					tokenize(s1, args);

					char command[MAX_LINE];
					strcpy(command, past_command[n - 1]);
					tokenize(command, args);
				}
			}
		}

		if (pid != 0) {
			addCommandToArr(args[0]);
		}

		if (pid == 0 && isSpecialCommand == 0) {
			execvp(args[0], args);
			fprintf(stderr, "\nCommand not recognised.\n");
			printf("\n");
			return 1;
		}

		if (has_ampersand == 0)
			wait(NULL);

	}
	
	return 0;
}
