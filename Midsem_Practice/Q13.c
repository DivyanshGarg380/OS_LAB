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
    
    - Implement First Come First Serve (FCFS) scheduling using pthreads:
        - Each process is represented as a thread
    
    - Use a mutex to ensure:
        - Only one thread executes at a time (CPU simulation)
    
    - Each thread:
        - Waits until its turn based on FCFS order
        - Prints:
            - Start time
            - Completion time
    
    - Finally display:
        - Waiting Time
        - Turnaround Time
*/

#include <stdio.h>
#include <pthread.h>

typedef struct {
    int id, at, bt, wt, tat, ct;
} process;

process p[10];
int n;
pthread_mutex_t lock;

void *run(void *arg) {
    int i = *(int *)arg;
    pthread_mutex_lock(&lock);

    int start = (i == 0) ? p[i].at : p[i-1].ct;
    if(start < p[i].at) start = p[i].at;

    p[i].ct = start + p[i].bt;
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
    }

    for(int i = 0; i < n - 1; ++i) {
        for(int j = i + 1; j < n; ++j) {
            if(p[i].at > p[j].at) {
                process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    pthread_mutex_init(&lock, NULL);
    pthread_t t[n];
    int idx[n];

    for(int i = 0; i < n; ++i) {
        idx[i] = i;
        pthread_create(&t[i], NULL, run, &idx[i]);
    }

    for(int i = 0; i < n; ++i) {
        pthread_join(t[i], NULL);
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