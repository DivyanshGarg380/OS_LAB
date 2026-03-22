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

    - Parent process:
        - Reads process details (ID, AT, BT, Priority)
        - Sends data to child using pipe
    
    - Parent creates child using fork() and executes ./schedulerExec
    - Child program:
        - Reads process data from pipe
        - Stores in shared memory
        - Implements Preemptive Priority Scheduling
    
    - Use:
        - Threads → represent processes
        - Semaphore → CPU control (only one process runs at a time)
    
    - At each time unit:
        - Select highest priority process
        - Preempt if needed
    
    - Output:
        - Execution timeline
        - CT, WT, TAT
*/

// Parent.c

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct {
    int id, at, bt, pr, rt, ct, wt, tat;
} process;

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    process p[10];

    for(int i = 0; i < n; ++i) {
        printf("ID AT BT PR: ");
        scanf("%d %d %d %d", &p[i].id, &p[i].at, &p[i].bt, &p[i].pr);
        p[i].rt = p[i].bt;
    }

    int fd[2];
    pipe(fd);

    if(fork() == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execl("./schedulerExec", "schedulerExec", NULL);
        perror("exec failed");
        exit(1);
    } else {
        close(fd[0]);
        write(fd[1], &n, sizeof(int));
        write(fd[1], p, sizeof(process)*n);
        close(fd[1]);
        wat(NULL);
    }
    

    return 0;
}

// schedulerExec.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

#define KEY 1234

typedef struct {
    int id, at, bt, pr, rt, ct, tat, wt;
} process;

Process p[10];
int n, time = 0, done = 0;

sem_t *sem;

void *run(void *arg) {
    int i = *(int *)arg;

    sem_wait(sem);
    p[i].rt--;
    printf("Time %d -> P%d\n", time_now, p[i].id);

    if (p[i].rt == 0) {
        p[i].ct = time + 1;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        done++;
    }

    sem_post(sem);
    return NULL;
}

int main() {
    read(STDIN_FILENO, &n, sizeof(int));
    read(STDIN_FILENO, p, sizeof(process) * n);

    sem = sem_open("/cpu_sum", O_CREAT, 0644, 1);
    while(done < n) {
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
            pthread_join(&t, NULL);
        }

        time++;
    }

    printf("\nID AT BT PR CT WT TAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d %d %d %d %d %d %d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].wt, p[i].tat);
    }

    sem_unlink("/cpu_sem");

    return 0;

}

