#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
	int fd[2];
	pid_t pid;
	char message[] = "Hi from child process";
	char buffer[100];

	if(pipe(fd) == -1) {
		printf("Pipe creation failed");
		return 1;
	}

	pid = fork();
	
	if(pid == 0) {
		close(fd[0]);
		write(fd[1], message, strlen(message) + 1);
		close(fd[1]);
	} else {
		close(fd[1]);
		read(fd[0], buffer, sizeof(buffer));
		printf("Parent recived: $s\n", buffer);
		close(fd[0]);
	}

	return 0;
}
