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
    Develop a C/C++ program where:
    
    - The program creates a Parent Process and a Child Process using fork().
    - Inter-Process Communication (IPC):
        - Use a pipe for communication between parent and child.
        - The parent process collects input from the user (no hardcoding allowed).
        - The parent sends the process details to the child through the pipe.
    
    - The parent reads the following details for each process:
        •	Process ID
        •	Arrival Time
        •	Burst Time
    
    - The child process:
        - Receives the input data through the pipe.
        - Executes a separate executable file (using exec() family functions).
        - The executable file implements:
            - Shortest Remaining Time First (SRTF) (Preemptive)
    
    - Inside the executable file:
        - Create two threads:
        - Thread 1: Performs the scheduling algorithm (SJF or SRTF).
        - Thread 2: Displays the output including:
            - Gantt Chart
            - Completion Time (CT)
            - Turnaround Time (TAT)
            - Waiting Time (WT)
    
    - Synchronization Requirements:
        - Use appropriate thread synchronization (mutex/semaphore) to ensure safe data sharing between threads.
        - Avoid race conditions while accessing shared scheduling data.
    
    - Scheduling Output:
        •	Display the Gantt Chart clearly.
        •	Calculate and display for each process:
        •	Completion Time (CT)
        •	Turnaround Time (TAT)
        •	Waiting Time (WT)
*/

// parent.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct {
    int id, at, bt;
} process;

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    process p[10];
    for (int i = 0; i < n; i++) {
        printf("Enter ID AT BT: ");
        scanf("%d %d %d", &p[i].id, &p[i].at, &p[i].bt);
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
        write(fd[1], p, sizeof(process) * n);
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}

// schedulerExec.c

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>

typedef struct {
    int id, at, bt, rt, ct, wt, tat;
} process;

process p[10];
int n;

int time = 0, done = 0;
pthread_mutex_t lock;

void *run(void *arg) {
    while(done < n) {
        pthread_mutex_lock(&lock);
        int pos = -1, mini = INT_MAX;
        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && p[i].rt > 0 && pt[i].rt < mini) {
                mini = p[i].rt;
                pos = i;
            }
        }

        if(pos != -1) {
            printf("Time %d -> P%d\n", time_now, p[pos].id);
            p[pos].rt--;

            if(p[pos].rt == 0) {
                done++;
                p[pos].ct = time_now + 1;
                p[pos].tat = p[pos].ct - p[pos].at;
                p[pos].wt = p[pos].tat - p[pos].bt;
            }
        }
        time++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *display(void *arg) {
    while(done < n);

    pthread_mutex_lock(&lock);
    printf("\nID AT BT CT WT TAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d %d %d %d %d %d\n",
               p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].wt, p[i].tat);
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    read(STDERR_FILENO, &n, sizeof(int));
    read(STDIN_FILENO, p, sizeof(process) * n);

    for (int i = 0; i < n; i++) {
        p[i].rt = p[i].bt;
    }

    pthread_mutex_init(&lock, NULL);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, run, NULL);
    pthread_create(&t2, NULL, display, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}