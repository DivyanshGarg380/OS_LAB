#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "myfifo"

int main() {
	int fd;
	int numbers[4];
	pid_t pid;
	
	mkfifo(FIFO_NAME, 0666);
	
	pid = fork();
	if(pid > 0) { // Parent aka Producer 
		fd = open(FIFO_NAME, O_WRONLY);
		printf("Producer: Enter 4 integers:\n");
		
		for(int i = 0; i < 4; ++i) scanf("%d" , &numbers[i]);
		
		write(fd, numbers, sizeof(numbers));
		printf("Producer: Numbers sent\n");
		close(fd);
	}
	else { // Child aka Consumer
		fd = open(FIFO_NAME, O_RDONLY);
		read(fd, numbers, sizeof(numbers));
		printf("Consumer: Numbers received\n");
		
		for(int i = 0; i < 4; ++i) printf("%d", numbers[i]);
		printf("\n");
		close(fd);
	}
		
	return 0;
}
