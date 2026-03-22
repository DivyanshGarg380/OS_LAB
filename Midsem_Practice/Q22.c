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

    - Parent:
        - Reads n, Time Quantum
        - Reads (ID, AT, BT)
        - Sends data to child using pipe
    
    - Child executes ./interruptExec
    - Inside interruptExec:
        - Thread 1 → implements Round Robin scheduling
        - Thread 2 → simulates interrupts every 3 time units
*/

// Parent.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct {
    int id, at, bt;
} process;

int main() {
    int n, tq;
    printf("Enter n: ");
    scanf("%d", &n);

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    process p[10];

    for(int i = 0; i < n; ++i) {
        printf("ID AT BT: ");
        scanf("%d %d %d", &p[i].id, &p[i].at, &p[i].bt);
    }

    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);

        execl("./interruptExec", "interruptExec", NULL);
        exit(1);
    } else {
        close(fd[0]);

        write(fd[1], &n, sizeof(int));
        write(fd[1], &tq, sizeof(int));
        write(fd[1], p, sizeof(P)*n);

        close(fd[1]);
        wait(NULL);
    }

    return 0;
}

// interruptExec.c

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int id, at, bt, rt;
} process;

process p[10];
int n, tq;

int time = 0, done = 0;
int interrupt = 0;

pthread_mutex_t lock;

void* interrupt_thread(void *arg) {
    while(done < n) {
        sleep(3);
        pthread_mutex_lock(&lock);
        interrupt = 1;
        printf("INTERRUPT at time %d\n", time_now);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *scheduler(void *arg) {
    while(done < n) {
        for(int i = 0; i < n; ++i) {
            pthread_mutex_lock(&lock);
            if(interrupt) {
                interrupt = 0;
                pthread_mutex_unlock(&lock);
                continue;
            }

            if(p[i].at <= time && p[i].rt > 0) {
                int exec = (p[i].rt > tq) ? tq : p[i].rt;
                printf("Time %d -> P%d\n", time_now, p[i].id);
                p[i].rt -= exec;
                time += exec;
                if(p[i].rt == 0) done++;
            }
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main() {
    read(0, &n, sizeof(int));
    read(0, &tq, sizeof(int));
    read(0, p, sizeof(process) * n);

    for(int i = 0; i < n; ++i) p[i].rt = p[i].bt;

    pthread_mutex_init(&lock, NULL);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, schedular, NULL);
    pthread_create(&t2, NULL, interrupt_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}