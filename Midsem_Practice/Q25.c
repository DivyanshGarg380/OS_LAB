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
    Write a C program using POSIX threads and shared memory (System V IPC) to simulate a Priority-based Round Robin CPU Scheduling algorithm.
    - Requirements:
        - Create a Process structure containing:
            - Process ID
            - Arrival Time (AT)
            - Burst Time (BT)
            - Priority (lower value = higher priority)
            - Completion Time (CT)
            - Waiting Time (WT)
            - Turnaround Time (TAT)
    
        - Use two threads:
            - Thread 1 (Input Thread):
                - Accepts number of processes n
                - Accepts time quantum
                - Accepts process details (AT, BT, Priority)
                - Stores all data in shared memory
            - Thread 2 (Scheduler Thread):
                - Implements Priority Scheduling
                - Among processes with the same highest priority, apply Round Robin Scheduling
                - Computes CT, WT, TAT
                - Displays:
                    - Gantt Chart
                    - Process Table (AT, BT, Priority, CT, WT, TAT)
                    - Average WT and TAT
    
        - Use shared memory (shmget, shmat) for storing process data.
        - Use proper synchronization (mutex) to avoid race conditions.
        - Do not use hardcoded values; all inputs must be taken at runtime.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <limits.h>

#define MAX 50
typedef struct {
    int id, at, bt, pr, ct, wt, tat;
} Process;

int n, quantum;
Process *p;

pthread_mutex_t lock;

void *input(void *arg) {
    pthread_mutex_lock(&lock);
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;

        printf("\nProcess P%d\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Burst Time: ");
        scanf("%d", &p[i].bt);
        printf("Priority (lower = higher): ");
        scanf("%d", &p[i].pr);
    }

    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);    
}

void *scheduler(void *arg) {
    pthread_mutex_lock(&lock);
    int rem[MAX];
    int time = 0, completed = 0;
    for(int i = 0; i < n; ++i) rem[i] = p[i].bt;

    printf("\nGantt Chart:\n");
    while(completed < n) {
        int best = INT_MAX;
        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && rem[i] > 0) {
                if(p[i].pr < best) {
                    best = p[i].pr;
                }
            }
        }

        if(best == INT_MAX) {
            time++;
            continue;
        }

        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && rem[i] > 0 && p[i].pr == best) {
                int exec = (rem[i] < quantum) ? rem[i] : quantum;
                printf("| P%d ", p[i].id);
                time += exec;
                rem[i] -= exec;
                if(rem[i] == 0) {
                    completed++;
                    p[i].ct = time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                }
            }
        }
    }

    printf("|\n");
    float totatWT = 0, totalTAT = 0;
    printf("\nProcess\tAT\tBT\tPR\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].wt, p[i].tat);

        totalWT += p[i].wt;
        totalTAT += p[i].tat;
    }

    printf("\nAverage WT = %.2f\n", totalWT / n);
    printf("Average TAT = %.2f\n", totalTAT / n);

    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);

    int shmid = shmget(1234, sizeof(Process) * MAX, 0666 | IPC_CREAT);
    p = (Process*)shmat(shmid, NULL, 0);

    pthread_create(&t2, NULL, input, NULL);
    pthread_join(t2, NULL);

    pthread_create(&t1, NULL, scheduler, NULL);
    pthread_join(t1, NULL);

    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}