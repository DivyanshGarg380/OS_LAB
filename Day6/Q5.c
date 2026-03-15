#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
	int fd[2];
	char write_msg[] = "I am Divyansh";
	char read_msg[100];

	if(pipe(fd) == -1){
		printf("Pipe creation failed\n");
		exit(1);
	}

	write(fd[1], write_msg, strlen(write_msg) + 1);
	read(fd[0], read_msg, sizeof(read_msg));
	
	printf("Message received: %s\n", read_msg);
	return 0;
}
