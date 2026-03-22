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
    Develop a C program to simulate a CPU Scheduling System with the following requirements:

    A) Parent Process
    - Read:
        - Number of processes n
        - Time Quantum TQ
        - For each process:
            - Process ID
            - Arrival Time
            - Burst Time
            - Priority
    
    - Store all process details in a shared memory segment using a struct array
    - Create a child process using fork()
    - Execute an external program ./schedulerExec using exec()
    
    B) Child Process (schedulerExec)
    - Access the shared memory created by the parent
    - Implement the following scheduling:

    - Hybrid Scheduling:
    - Use Preemptive Priority Scheduling
    - If multiple processes have same priority → use Round Robin (TQ)

    C) Execution Model
    - Each process must be represented using pthreads
    - Use:
        - Semaphore → simulate CPU (only one process runs at a time)
        - Mutex (optional) → protect shared updates

    - At each unit of time:
        - Select process with highest priority
        - If tie → apply Round Robin

    - Preempt currently running process if:
        - Higher priority process arrives
    
    D) Output
    - Display execution timeline:
    - Display table

    E) Constraints
    - No busy waiting
    - Proper synchronization required
    - All data must be dynamically handled (no hardcoding)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define KEY 1234

typedef struct {
    int id, at, bt, rt, pr, ct, wt, tat;
} process;

int main() {
    int n, tq;

    printf("Enter n: ");
    scanf("%d", &n);

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    int shmid = shmget(KEY, sizeof(process) * (n + 2), 0666 | IPC_CREAT);
    process *p = (process *)shmat(shmid, NULL, 0);

    p[0].id = tq;
    p[1].id = n;

    for (int i = 0; i < n; i++) {
        printf("ID AT BT PR: ");
        scanf("%d %d %d %d",
              &p[i+2].id, &p[i+2].at, &p[i+2].bt, &p[i+2].pr);

        p[i+2].rt = p[i+2].bt;
    }

    if(fork() == 0) {
        execl("./schedulerExec", "schedulerExec", NULL);
        perror("exec failed");
        exit(1);
    }

    wait(NULL);
    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

// schedulerExec.c
#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

#define KEY 1234

typedef struct {
    int id, at, bt, rt, pr, ct, wt, tat;
} process;

process *p;
int n , tq;
int time = 0, done = 0;
int rr = 0;
sem_t *sem;

void *run(void *arg) {
    int i = *(int*)arg;
    
    sem_wait(sem);
    int exec = (p[i].rt > tq) ? tq : p[i].rt;
    printf("Time %d -> P%d\n", time_now, p[i].id);

    p[i].rt -= exec_time;
    time += exec_time;

    if(p[i].rt == 0) {
        p[i].ct = time_now;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        done++;
    }

    sem_post(sem);
    return NULL;
}

int main() {
    int shmid = shmget(KEY, 0, 0666);
    p = (process *)shmat(shmid, NULL, 0);

    tq = p[0].id;
    n = p[1].id;
    
    sem = sem_open("/cpu", O_CREAT, 0644, 1);
    while(done < n) {
        int best = INT_MAX;
        int found = 0;

        for(int i = 2; i < n + 2; ++i) {
            if(p[i].at <= time && p[i].rt > 0) {
                if(p[i].pr < best) {
                    best = p[i].pr;
                    found = 1;
                }
            }
        }

        if(!found) {
            time++;
            continue;
        }

        int candidates[10], cnt = 0;
        for(int i = 2; i < n + 2; ++i) {
            if(p[i].at <= time && p[i].rt > 0 && p[i].pr == best) {
                candidates[cnt++] = i;
            }
        }

        int idx = candidates[rr%cnt];
        rr++;

        pthread_t t;
        int idx = i;
        pthread_create(&t, NULL, run, &idx);
        pthread_join(&t, NULL);
    }

    printf("\nID AT BT PR CT WT TAT\n");
    for (int i = 2; i < n + 2; i++) {
        printf("%d %d %d %d %d %d %d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].wt, p[i].tat);
    }

    sem_unlink("/cpu_sched");
    shmdt(p);

    return 0;
}