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
    Develop a C program where:

    - The main thread reads:
        - Number of processes n
        - For each process:
            - Process ID
            - Arrival Time
            - Burst Time
            - Priority
    
    - Implement Preemptive Priority Scheduling using pthreads:
        - At every unit of time:
            - Select process with highest priority (lowest value) among arrived
            - Preempt currently running process if needed
    
    - Each process is represented as a thread
    - Use a mutex to simulate CPU:
        - Only one thread executes at a time
    
    - Display:
        - Execution timeline
        - Completion Time
        - Waiting Time
        - Turnaround Time
*/

#include <stdio.h>
#include <pthread.h>
#include <limits.h>

typedef struct {
    int id, at, bt, rt, pr;
    int ct, wt, tat;
} process;

process p[10];
int n;
int time = 0, completed = 0;

pthread_mutex_t lock;

void *run(void *arg) {
    int i = *(int *)arg;
    pthread_mutex_lock(&lock);

    p[i].rt--;
    printf("Time %d -> Process %d running\n", time, p[i].id);
    if(p[i].rt == 0) {
        completed++;
        p[i].ct = time + 1;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter ID, AT, BT, Priority: ");
        scanf("%d %d %d %d", &p[i].id, &p[i].at, &p[i].bt, &p[i].pr);
        p[i].rt = p[i].bt;
    }

    pthread_mutex_init(&lock, NULL);
    while(completed < n) {
        int pos = -1, best = INT_MAX;
        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && p[i].rt > 0 && p[i].pr < best) {
                best = p[i].pr;
                pos = i;
            }
        }

        if(pos != -1) {
            pthread_t t;
            int idx = pos;
            pthread_create(&t, NULL, run, &idx);
            pthread_join(t, NULL);
        }
        
        time++;
    }

    printf("\nID\tAT\tBT\tPR\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].wt, p[i].tat);
    }

    pthread_mutex_destroy(&lock);
    return 0;

}