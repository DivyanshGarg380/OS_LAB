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

    - Parent creates a message queue using key (key_t)1234.
    - Parent creates 3 child processes:
        - Child 1 (Producer)
            - Takes N integers as input
            - Sends them to message queue (mtype = 1)
        
        - Child 2 (Worker A)
            - Receives numbers (mtype = 1)
            - Computes square
            - Sends result back with mtype = 2
        
        - Child 3 (Worker B)
            - Receives numbers (mtype = 1)
            - Computes cube
            - Sends result back with mtype = 3
        
        - Parent
            - Receives:
            - Squares (mtype = 2)
            - Cubes (mtype = 3)
            - Prints:
                - Square of X = Y
                - Cube of X = Z
    
        - Synchronization
            - For each number:
                - Square MUST be printed before cube
            - Use semaphore (sem_init) to enforce ordering
        
        - Termination
            - Use -1 to signal end
            - Must terminate all processes cleanly
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/shm.h>

struct msgbuf {
    long mtype;
    int num;
};

typedef struct {
    sem_t order;
} shared;

int main() {
    int msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    int shmid = shmget((key_t)5678, sizeof(shared), 0666 | IPC_CREAT);
    shared* shm = (shared*)shmat(shmid, NULL, 0);

    sem_init(&shm->order, 1, 0);

    if(fork() == 0) {
        // producer
        struct msgbuf msg;
        int n, x;

        printf("Enter n: ");
        scanf("%d", &n);

        for(int i = 0; i < n; ++i) {
            scanf("%d", &x);
            msg.type = 1;
            msg.num = x;
            msgsnd(msgid, &msg, sizeof(int), 0);
        }

        msg.num = -1;
        msgsnd(msgid, &msg, sizeof(int), 0);
        exit(0);
    }

    if(fork() == 0) {
        // Square
        struct msgbuf msg;
        while(1) {
            msgrcv(msgid, &msg, sizeof(int), 1, 0);
            if(msg.num == 1) {
                msgsnd(msgid, &msg, sizeof(int), 0);
                break;
            }

            int res = msg.num * msg.num;
            struct msgbuf out = {2, res};
            msgsnd(msgid, &out, sizeof(int), 0);
            sem_post(&shm->order);
        }
        exit(0);
    }

    if(fork() == 0) {
        // cube
        struct msgbuf msg;
        while(1) {
            msgrcv(msgid, &msg, sizeof(int), 1, 0);
            if(msg.num == -1) {
                msgsnd(msgid, &msg, sizeof(int), 0);
                break;
            }

            sem_wait(&shm->order);
            int res = msg.num * msg.num * msg.num;
            struct msgbuf out = {3, res};
            msgsnd(msgid, &out, sizeof(int), 0);
        }
        exit(0);
    }

    struct msgbuf msg;
    int done = 0;
    while(!done) {
        msgrcv(msgid, &msg, sizeof(int), 0, 0);

        if(msg.mtype == 1) continue;

        if(msg.num == -1) {
            done = 1;
            continue;
        }

        if(msg.mtype == 2) {
            printf("Square = %d\n", msg.num);
        } else if(msg.mtype == 3) {
            printf("Cube = %d\n", msg.num);
        }
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    msgctl(msgid, IPC_RMID, NULL);
    sem_destroy(&shm->order);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}