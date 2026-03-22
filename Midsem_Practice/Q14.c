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
    
    - Implement Non-Preemptive Shortest Job First (SJF) scheduling using pthreads:
        - At any time, select the process with minimum burst time among arrived processes
    
    - Each process is represented as a thread
    - Use a mutex to simulate CPU access:
        - Only one thread executes at a time
    
    - Each thread prints:
        - Start time
        - Completion time
    
    - Finally display:
        - Waiting Time
        - Turnaround Time
*/

#include <stdio.h>
#include <pthread.h>
#include <limits.h>

typedef struct {
    int id, at, bt, ct, wt, tat, done;
} process;

process p[10];
int n;
int time = 0;

pthread_mutex_t lock;

void *run(void *arg) {
    int i = *(int *)arg;

    pthread_mutex_lock(&lock);

    if(time < p[i].at) time = p[i].at;

    int start = time;
    time += p[i].bt;

    p[i].ct = time;
    p[i].tat = p[i].ct - p[i].at;
    p[i].wt = p[i].tat - p[i].bt;

    printf("Process %d runs from %d to %d\n", p[i].id, start, p[i].ct);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter ID, AT, BT: ");
        scanf("%d %d %d", &p[i].id, &p[i].at, &p[i].bt);
        p[i].done = 0;
    }

    pthread_mutex_init(&lock, NULL);

    pthread_t t[10];
    int idx[10], cnt = 0;

    while(cnt < n) {
        int mini = INT_MAX, pos = -1;

        for(int i = 0; i < n; ++i) {
            if(!p[i].done && p[i].at <= time && p[i].bt < mini) {
                mini = p[i].by;
                pos = i;
            }
        }

        if(pos == -1) {
            time++;
            continue;
        }

        idx[pos] = pos;
        pthread_create(&t[pos], NULL, run, &idx[pos]);
        pthread_join(&t[pos], NULL);

        p[pos].done = 1;
        cnt++;
    }

    printf("\nID\tAT\tBT\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].wt, p[i].tat);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}