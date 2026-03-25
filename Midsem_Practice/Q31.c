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

    - Parent
        - Creates message queue (key_t)1234
        - Takes:
            - n processes
            - burst times
            - time quantum q

        - Sends each process as:
            - {process_id, burst_time} with mtype = 1
    
    - Child (Scheduler Process)
        - Receives all processes from queue
        - Implements Round Robin Scheduling
        - Uses 2 threads:
            - Thread 1 (CPU Executor)
                - Picks process
                - Executes for q
                - Updates remaining time
                - Sends execution info to parent via message queue (mtype = 2)
            
            - Thread 2 (Logger / Completion Handler)
                - Monitors completed processes
                - When a process finishes:
                    - Calculates:
                        - CT, TAT, WT
                - Sends to parent (mtype = 3)
    
    - Parent
        - Receives:
            - mtype = 2 → execution updates
            - mtype = 3 → completion stats
        
        - Prints output
        
    - Termination
        - Use burst = -1 as termination signal
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/wait.h>

#define MAX 10

struct msgbuf {
    long type;
    int pid;
    int burst;
};

int n, q;
int rem[MAX], burst[MAX];
int completed = 0, time = 0;

pthread_mutex_t lock;
int msgid;

void *executor(void *arg) {
    while(completed < n) {
        for(int i = 0; i < n; ++i) {
            if(rem[i] > 0) {
                pthread_mutex_lock(&lock);
                int exec = (rem[i] > q) ? q : rem[i];
                rem[i] -= exec;
                time += exec;

                struct msgbuf msg = {2, i+1, rem[i]};
                msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

                pthread_mutex_unlock(&lock);
                sleep(1);
            }
        }
    }
    return NULL;
}

void *logger(void *arg) {
    int visited[MAX] = {0};
    while(completed < n) {
        for(int i = 0; i < n; ++i) {
            pthread_mutex_lock(&lock);
            if(rem[i] == 0 && !visited[i]) {
                visited[i] = 1;
                completed++;
                int ct = time;
                int tat = ct;
                int wt = tat - burst[i];

                struct msgbuf msg = {3, i+1, wt};
                msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
            }
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main() {
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    printf("Enter n: ");
    scanf("%d", &n);

    printf("Enter burst times:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &burst[i]);
    }

    printf("Enter quantum: ");
    scanf("%d", &q);

    struct msgbuf msg;
    for(int i = 0; i < n; ++i) {
        msg.type = 1;
        msg.pid = i + 1;
        msg.burst = burst[i];
        msgsnd(msgid, &msg, sizeof(msg), 0);
    }

    pid_t pid = fork();
    if(pid == 0) {
        // scheduler
        for(int i = 0; i < n; ++i) {
            msgrcv(msgid, &msg, sizeof(msg), 1, 0);
            rem[i] = msg.burst;
        }

        pthread_t t1, t2;
        pthread_mutex_init(&lock, NULL);

        pthread_create(&t1, NULL, executor, NULL);
        pthread_create(&t2, NULL, logger, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        pthread_mutex_destroy(&lock);
        exit(0);
    }

    // parent
    int done = 0;
    while(done < n) {
        msgrcv(msgid, &msg, sizeof(msg), 0, 0);
        if(msg.type == 2) printf("Process %d remaining = %d\n", msg.pid, msg.burst);
        else if(msg.type == 3){
            printf("Process %d WT = %d\n", msg.pid, msg.burst);
            done++;
        }
    }

    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);
}