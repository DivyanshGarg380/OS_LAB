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
    - The parent process:
        - Reads number of processes n
        - For each process reads:
            - Process ID
            - Arrival Time
            - Burst Time
        - Reads Time Quantum (TQ)
        - Stores all data in shared memory (struct array)
    
    - The parent creates a child process using fork() and executes ./rrExec
    - The child program:
        - Accesses the shared memory
        - Implements Round Robin Scheduling using pthreads
        - Each process is represented as a thread
    
    - Use semaphore to simulate CPU:
        - Only one process executes at a time
    
    - At each turn:
        - Execute process for TQ or remaining time
        - Move process to end of queue if not finished
    
    - Display:
        - Execution timeline
        - Completion Time (CT)
        - Waiting Time (WT)
        - Turnaround Time (TAT)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define KEY 1234

typedef struct {
    int id, at, bt, rt, ct, wt, tat;
} process;

int main() {
    int n , tq;
    printf("Enter n: ");
    scanf("%d", &n);

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    int shmid = shmget(KEY, sizeof(process) * (n+2), 0666 | IPC_CREAT);
    process *p = (process*)shmat(shmid, NULL, 0);

    p[0].id = tq;
    p[1].id = n;

    for(int i = 2; i < n; ++i) {
        printf("ID AT BT: ");
        scanf("%d %d %d", &p[i].id, &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
    }

    if(fork() == 0) {
        execl("./rrExec", "rrExec", NULL);
        perror("exec failed");
        exit(1);
    }
    
    wait(NULL);
    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

// rrExec.c

#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define KEY 1234

typedef struct {
    int id, at, bt, rt, ct, wt, tat;
} process;

process *p;
int n , tq;
int time = 0, done = 0;

sem_t *sem;
void *run(void *arg) {
    int i = *(int *)arg;

    // locking process now
    sem_wait(sem);

    int exec_time = (p[i].rt > tq) ? tq : p[i].rt;
    printf("Time %d -> P%d\n", time, p[i].id);

    p[i].rt -= exec_time;
    time += exec_time;

    if(p[i].rt == 0) {
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        ++done;
    } 

    // unlocking process now
    sem_post(sem);
    return NULL;
}

int main() {
    int shmid = shmget(KEY, 0, 0666);
    p = (process *)shmat(shmid, NULL, 0);

    tq = p[0].id;
    n = p[1].id;

    tq = p[n].id;
    sem = sem_open("/cpu", O_CREAT, 0644, 1);
    while(done < n) {
        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && p[i].rt > 0) {
                pthread_t t;
                int idx = i;
                pthread_create(&t, NULL, run , &idx);
                pthread_join(&t, NULL);
            }
        }
        time++;
    }

    printf("\nID AT BT CT WT TAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d %d %d %d %d %d\n",
               p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].wt, p[i].tat);
    }

    sem_unlink("/cpu");
    return 0;
}