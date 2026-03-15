#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 100

int main() {
	int shmid;
	char *shared_mem;
	
	shmid = shmget((key_t)1234, SIZE, 0666 | IPC_CREAT);
	
	if(shmid == -1) {
		printf("Shared memory creation failed\n");
		exit(1);
	}

	shared_mem = (char*) shmat(shmid, NULL , 0);
	if(shared_mem == (char*) -1){ 
		printf("Error attaching\n");
		exit(1);
	}

	pid_t pid = fork();
	
	if(pid > 0) {
		char buffer[SIZE];
		printf("Producer: Enter a sentence");
		fgets(buffer, SIZE, stdin);
		
		strcpy(shared_mem, buffer);
		printf("Producer: Data written to shared memory\n");
		wait(NULL);
	}

	else {
		sleep(1);
		printf("Consumer: Data read from shared memory\n");
		printf("%s", shared_mem);
	}

	shmdt(shared_mem);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
