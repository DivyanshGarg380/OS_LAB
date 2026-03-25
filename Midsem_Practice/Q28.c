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

    - Parent creates 2 child processes:
        - Child 1 (Producer)
            - Reads N numbers from user
            - Writes them into shared memory
        
        - Child 2 (Scheduler + Consumer)
            - Reads numbers from shared memory
            - Treat each number as burst time of a process
            - Applies Round Robin Scheduling (q given by user)
            - Prints execution order and remaining time
    
    - Use:
        - Shared Memory ((key_t)1234)
        - Semaphores (sem_init) for sync
            - Producer writes first
            - Consumer waits until data is ready
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

#define MAX 10

typedef struct {
    int n;
    int burst[MAX];
    sem_t ready;
} shared;

int main() {
    int shmid = shmget((key_t)1234, sizeof(shared), 0666 | IPC_CREAT);
    shared* shm = (shared*)shmat(shmid, NULL, 0);

    sem_init(&shm->ready, 1, 0);
    pid_t pid1 = fork();
    if(pid1 == 0) {
        // producer
        printf("Enter number of processes: ");
        scanf("%d", &shm->n);

        printf("Enter burst times:\n");
        for (int i = 0; i < shm->n; i++) {
            scanf("%d", &shm->burst[i]);
        }

        sem_post(&shm->ready);
        exit(0);
    }

    pid_t pid2 = fork();
    if(pid2 == 0) {
        // consumer
        sem_wait(&shm->ready);
        int q;
        printf("Enter time quantum: ");
        scanf("%d", &q);

        int rem[MAX], time = 0;
        for(int i = 0; i < shm->n; ++i) rem[i] = shm->burst[i];

        int done = 0;
        while(done < shm->n) {
            for(int i = 0; i < shm->n; ++i) {
                if(rem[i] > 0) {
                    int exec = (rem[i] > q) ? q : rem[i];
                    time += exec;
                    rem[i] -= exec;
                    
                    printf("P%d executed for %d, remaining = %d\n", i+1,exec, rem[i]);
                    if(rem[i] == 0) {
                        int ct = time;
                        int tat = ct - 0; // AT is 0 for all
                        int wt = tat - shm->burst[i];
                        printf("P%d -> CT=%d TAT=%d WT=%d\n", i+1, ct, tat, wt);
                        done++;
                    }
                }
            }
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    sem_destroy(&shm->ready);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

