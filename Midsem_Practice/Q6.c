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
    - A parent process creates two child processes using fork()
    - Parent sends numbers from 1 to 10 to Child 1 using a pipe
    - Child 1 filters even numbers and sends them to Child 2 using another pipe
    - Child 2 prints the received numbers
    - Use semaphore to ensure Child 2 executes only after Child 1 finishes
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    sem_t *sem = sem_open("/sync_sem", O_CREAT, 0644, 0);
    pid_t c1 = fork();

    if(c1 == 0) { // child 1
        close(p1[1]);
        close(p2[0]);

        int num;
        while(read(p1[0], &num, sizeof(int)) > 0) {
            if(num % 2 == 0) {
                write(p2[1], &num, sizeof(int));
            }
        }

        close(p1[0]);
        close(p2[1]);

        sem_post(sem);
        exit(0);
    }

    pid_t c2 = fork();
    if(c2 == 0) { // child 2
        close(p1[0]); 
        close(p1[1]);
        close(p2[1]);
        sem_wait(sem);
        int num;
        printf("Even numbers:\n");
        while(read(p2[0], &num, sizeof(int)) > 0) {
            printf("%d ", num);
        }

        printf("\n");
        close(p2[0]);
        exit(0);
    }

    // parent
    close(p1[0]);
    close(p2[0]);
    close(p2[1]);

    for(int i = 1; i <= 10; ++i) {
        write(p1[1], &i, sizeof(int));
    }
    close(p1[1]);
    wait(NULL);
    wait(NULL);
    sem_unlink("/sync_sem");
    return 0;
}