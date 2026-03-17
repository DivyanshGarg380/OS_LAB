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
    Reader-Writer Problem using Shared Memory and Semaphores

    Develop a C program to simulate the Reader-Writer problem using Shared Memory and Semaphores with the following requirements:

        a) The Parent process reads the number of readers and writers along with the number of operations to be performed. It then displays the entered details in a tabular format.

        b) Create two separate C/C++ programs, compiled independently as executable files named:
            • readerSM
            • writerSM

            The programs should use shared memory for data exchange:
                • Readers read from shared memory
                • Writers update shared memory

            The input must be taken dynamically and should not be hardcoded.

        c) The parent process creates multiple child processes and ensures proper synchronization between readers and writers using semaphores.

*/

// Parent -> main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int r, w, ops;
    printf("Enter number of readers: ");
    scanf("%d", &r);

    printf("Enter number of writers: ");
    scanf("%d", &w);

    printf("Enter number of operations: ");
    scanf("%d", &ops);

    printf("\nReaders: %d | Writers: %d | Operations: %d\n", r, w, ops);

    for(int i = 0; i < r; ++i) {
        if(fork() == 0) {
            execl("./readerSM", "readerSM", NULL);
            perror("exec reader failed");
            exit(1);
        }
    }

    for(int i = 0; i < w; ++i) {
        if(fork() == 0) {
            execl("./writerSM", "writerSM", NULL);
            perror("exec writer failed");
            exit(1);
        }
    }

    for(int i = 0; i < r + w; ++i) wait(NULL);
    return 0;
}

// readerSM.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    int shmid = shmget(1234, sizeof(int)  * 2, 0666 | IPC_CREAT);
    int *data = (int *)shmat(shmid, NULL, 0);

    sem_t *mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    sem_t *wrt = sem_open("/wrt", O_CREAT, 0644, 1);

    sem_wait(mutex);
    data[1]++;

    if(data[1] == 1) sem_wait(wrt);
    sem_post(mutex);

    printf("Reader %d read value: %d\n", getpid(), data[0]);

    sem_wait(mutex);
    data[1]--;

    if(data[1] == 0) sem_post(wrt);

    sem_post(mutex);
    shmdt(data);
    return 0;
}

// writerSM.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    int shmid = shmget(1234, sizeof(int) * 2, 0666 | IPC_CREAT);
    int *data = (int *)shmat(shmid, NULL, 0);

    sem_t *wrt = sem_open("/wrt", O_CREAT, 0644, 1);
    sem_wait(wrt);

    data[0] += 10;
    printf("Writer %d updated value to: %d\n", getpid(), data[0]);
    sem_post(wrt);
    shmdt(data);
    return 0;
}
