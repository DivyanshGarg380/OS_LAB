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

    - A parent process creates 3 child processes using fork()
    - A shared variable (initial value = 0) is stored in shared memory
    - Each child increments the variable 500 times
    - Requirements:
      - First, run without synchronization and display final value
      - Then, use semaphore to avoid race condition and display correct value
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fcntl.h>

#define N 3
int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    int *shared = (int *)shmat(shmid, NULL, 0);

    *shared = 0;
    printf("Without Synchronization:\n");
    for(int i = 0; i < N; ++i) {
        if(fork() == 0) {
            for(int j = 0; j < 500; j++) {
                (*shared)++;
            }
            exit(0);
        }
    }

    for(int i = 0; i < N; ++i) wait(NULL);
    printf("Final value: %d\n", *shared);

    *shared = 0;
    printf("\nWith Synchronization:\n");
    sem_t *sem = sem_open("/sem_sync", O_CREAT, 0644, 1);
    for(int i = 0; i < N; ++i) {
        if(fork() == 0) {
            for(int j = 0; j < 500; ++j){
                sem_wait(sem);
                (*shared)++;
                sem_post(sem);
            }
            exit(0);
        }
    }

    for(int i = 0; i < N; ++i) wait(NULL);

    printf("Final value: %d\n", *shared);
    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);
    sem_unlink("/sem_sync");
    return 0;
}
