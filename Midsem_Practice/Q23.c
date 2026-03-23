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
    BOUNDED BUFFER + SHARED MEMORY + MULTI-PROCESS
    
    Develop a C program where:
    - Parent:
        - Creates shared memory buffer of size 5
        - Initializes semaphores:
            - empty = 5
            - full = 0
            - mutex = 1
    
    - Create:
        - 2 producer processes (using fork)
        - 2 consumer processes

    - Producers generate even numbers only
    - Consumers consume and print only if number is divisible by 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SIZE 5

typedef struct {
    int buffer[SIZE];
    int in, out;
} shared;

int main() {
    int shmid = shmget(1234, sizeof(shared), 0666 | IPC_CREAT);
    shared *s = (shared *)shmat(shmid, NULL, 0);

    s->in = s->out = 0;
    sem_t *empty = sem_open("/empty", O_CREAT, 0666, SIZE);
    sem_t *full = sem_open("/full", O_CREAT, 0644, 0);
    sem_t *mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    for(int p = 0; p < 2; ++p) { // producers
        if(fork() == 0) {
            for(int i = 0; i < 10; ++i) {
                int item = 2 * (rand() % 50);
                sem_wait(empty);
                sem_wait(mutex);

                s->buffer[s->in] = item;
                s->in = (s->in + 1) % SIZE;
                printf("Produced: %d\n", item);
                sem_post(mutex);
                sem_post(full);
                sleep(1);
            }
            exit(0);
        }
    }

    for(int c = 0; c < 2; ++c) { // consumers
        if(fork() == 0) {
            for(int i = 0; i < 10; ++i) {
                sem_wait(full);
                sem_wait(mutex);

                int item = s->buffer[s->out];
                s->out = (s->out + 1) % SIZE;

                if(item % 4 == 0) printf("Consumed: %d\n", item);
                sem_post(mutex);
                sem_post(empty);
                sleep(1);
            }
            exit(0);
        }
    }
    
    for(int i = 0; i < 4; ++i) wait(NULL);

    sem_unlink("/empty");
    sem_unlink("/full");
    sem_unlink("/mutex");
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}