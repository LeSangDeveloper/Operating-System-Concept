#include<stdio.h>

#define MIN_PID 300
#define MAX_PID 5000

/*slots for Processes*/
int pid_array[4701];
int last;

int allocate_map(void) {
  	int i;
  	for (i = 0; i <=(MAX_PID - MIN_PID); i++) {
    		pid_array[i] = 0;
  	}
  	last = 0;
  	if (pid_array[4700] == 0) {
    		return 1;           // return 1 if successful
  	} else {
  	  return -1;          // return —1 if unsuccessful
 	 }	
	return 1;
}

int allocate_pid(void) {
    int i = 0;        // Loop counter
    int next;         // Pointer to next pid
    if (last == MAX_PID) {
      next = MIN_PID;
    } else {
      next = last + 1;
    }
    while ((i <= (MAX_PID - MIN_PID)) && (pid_array[next] == 1)) {
      ++i;
      ++next;
      if (next > MAX_PID) {
        next = MIN_PID;
      }
    }
    if (i == (MAX_PID - MIN_PID) + 1) {
        return -1;      // return — 1 if unable to allocate a pid (all pids are in use)
    }
    pid_array[next] = 1;
    last = next;
    return last + 300;
}

void release_pid(int pid) {
	pid_array[pid - 300] = 0;
}

int PID_slot_number(int pid) {
	return pid_array[pid - 300];
}

int main() {
	int min_pid = allocate_map();
	printf("%d\n", min_pid);
	int processID = allocate_pid();
	printf("%d\n", processID);
	printf("%d\n", PID_slot_number(processID));
	release_pid(processID);
	printf("%d\n", PID_slot_number(processID));
	return 0;
}
