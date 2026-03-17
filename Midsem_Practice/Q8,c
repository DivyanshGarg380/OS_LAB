/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

/*
    Write a C program where:
    - A parent process creates a child using fork()
    - The child process executes the command ls using execvp()
    - The output of ls is redirected to the parent using a pipe
    - The parent counts and prints the number of files
    - Constraints:
      - Do not use system()
      - Properly handle pipe redirection before exec
*/

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if(pid == 0) {
        // child
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        char *args[] = {"ls", NULL};
        execvp("ls", args);

        perror("exec failed");
        exit(1);
    } else {
        // parent
        close(fd[1]);
        char buffer[1024];
        int cnt = 0;

        while(read(fd[0], buffer, sizeof(buffer)) > 0) {
            for(int i = 0; i < strlen(buffer); ++i) {
                if(buffer[i] == '\n') cnt++;
            }
        }
        close(fd[0]);
        wait(NULL);
        printf("Number of files: %d\n", cnt);
    }
    return 0;
}
