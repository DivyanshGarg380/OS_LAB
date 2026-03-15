#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {
	int shmid;
	char *shared_mem;

	shmid = shmget((key_t)1234, sizeof(char), 0666 | IPC_CREAT);
	if(shmid == -1) {
		printf("Shared memory creation failed\n");
		exit(1);
	}

	shared_mem = (char*) shmat(shmid, NULL, 0);
	if(shared_mem == (char*) -1) {
		printf("Shared memory attachment failed\n");
		exit(1);
	}

	pid_t pid = fork();
	
	if(pid > 0) {
		char ch;
		printf("Parent: Enter an alphabet");
		scanf("%c", &ch);
		
		*shared_mem = ch;
		sleep(1);
		printf("Parent: Child responded with %c\n", *shared_mem);
	}

	else {
		sleep(1);
		char received = *shared_mem;
		printf("Child: Received %c\n", received);
		received += 1;
		*shared_mem = received;
		printf("Child: Send next alphabet %c\n", received);
	}
	
	shmdt(shared_mem);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
