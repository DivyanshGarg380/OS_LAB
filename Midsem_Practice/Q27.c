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
    Design a C program that simulates a bounded buffer (size = 5) using:

    - Shared Memory
    - Unnamed Semaphores (sem_init)
    - fork() (multiple producers & consumers)
    
    - Requirements:
        - Parent creates shared memory containing:
        - Buffer array of size 5
        - in, out indices
        - Semaphores:
            - empty → initialized to 5
            - full → initialized to 0
            - mutex → initialized to 1
    
        - Parent creates:
            - 2 Producer processes
            - 2 Consumer processes
    
        - Each Producer:
            - Produces 5 items (numbers)
            - Writes into buffer

        - Consumes 5 items

    - Output must clearly show:
    - Producer X produced Y
    - Consumer X consumed Y
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SIZE 5
typedef struct {
    int buffer[SIZE];
    int in, out;
    sem_t empty, full, mutex;
} shared;

int main() {
    int shmid = shmget((key_t)1234, sizeof(shared), 0666 | IPC_CREAT);
    shared *data = (shared*)shmat(shmid, NULL, 0);

    data->in = 0;
    data->out = 0;

    sem_init(&data->empty, 1, SIZE);
    sem_init(&data->fullm 1, 0);
    sem_init(&data->mutex, 1, 1);

    // 2 producers
    for(int p = 0; p < 2; ++p) {
        if(fork() == 0) {
            for(int i = 0; i < 5; ++i) {
                int item = rand() % 100;
                sem_wait(&data->empty);
                sem_wait(&data->mutex);

                data->buffer[data->in] = item;
                printf("Producer %d produed %d\n", p + 1, item);
                data->in = (data->in + 1) % SIZE;

                sem_post(&data->mutex);
                sem_post(&data->full);
                sleep(1);
            }
            exit(0);
        }
    }

    // 2 consumers
    for(int c = 0; c < 2; ++c) {
        if(fork() == 0) {
            for(int i = 0; i < 5; ++i) {
                sem_wait(&data->full);
                sem_wait(&data->mutex);

                int item = data->buffer[data->out];
                printf("Consumer %d consumed %d\n", c + 1, item);
                data->out = (data->out + 1) % SIZE;

                sem_post(&data->mutex);
                sem_post(&data->empty);
                sleep(1);
            }
            exit(0);
        }
    }

    for(int i = 0; i < 4; ++i) wait(NULL);
    sem_destroy(&shm->empty);
    sem_destroy(&shm->full);
    sem_destroy(&shm->mutex);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

