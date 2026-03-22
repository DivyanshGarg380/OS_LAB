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
    MULTI-LEVEL QUEUE SCHEDULING
    - Parent sends processes
    - Child executes ./mlqExec
    - Inside:
        - Queue 1 → Priority Scheduling
        - Queue 2 → Round Robin

    - Rule:
        - High priority → Queue 1
        - Low priority → Queue 2

    - Threads:
        - Thread 1 → scheduling
        - Thread 2 → output
*/

// parent.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
    int id, at, bt, pr;
} process;

int main() {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    process p[10];

    for (int i = 0; i < n; i++) {
        printf("ID AT BT PR: ");
        scanf("%d %d %d %d", &p[i].id, &p[i].at, &p[i].bt, &p[i].pr);
    }

    if (fork() == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);

        execl("./mlqExec", "mlqExec", NULL);
        exit(1);
    } else {
        close(fd[0]);

        write(fd[1], &n, sizeof(int));
        write(fd[1], p, sizeof(P)*n);

        close(fd[1]);
        wait(NULL);
    }

    return 0;
}

// mlqExec.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>

typedef struct {
    int id, at, bt, rt, pr, ct, wt, tat;
} process;

process p[10];
int n;
int time = 0, done = 0;
int tq = 2;
pthread_mutex_t lock;

void* schedular(void *arg){
    while(done < r) {
        pthread_mutex_lock(&lock);
        int pos = -1;
        int best = INT_MAX;

        // Q1 -> Priority
        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && p[i].rt > 0 && p[i].pr <= 2) {
                if(p[i].pr < best) {
                    best = p[i].pr;
                    pos = i;
                }
            }
        }

        if(pos != -1) {
            printf("Time %d -> P%d (Priority)\n", time_now, p[pos].id);

            p[pos].rt--;
            time_now++;

            if (p[pos].rt == 0) {
                done++;
                p[pos].ct = time_now;
                p[pos].tat = p[pos].ct - p[pos].at;
                p[pos].wt = p[pos].tat - p[pos].bt;
            }
        } else {
            // Q2 -> Round Robin
            for(int i = 0; i < n; ++i) {
                if(p[i].at <= time && p[i].rt > 0 && p[i].pr > 2) {
                    int exec = (p[i].rt > tq) ? tq : p[i].rt;
                    printf("Time %d -> P%d (RR)\n", time_now, p[i].id);
                    p[i].rt -= exec;
                    time += exec;
                    if(p[i].rt == 0) {
                        done++;
                        p[i].ct = time_now;
                        p[i].tat = p[i].ct - p[i].at;
                        p[i].wt = p[i].tat - p[i].bt;
                    }
                    break;
                }
            }
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* display(void *arg) {
    while (done < n);

    pthread_mutex_lock(&lock);

    printf("\nID AT BT PR CT WT TAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d %d %d %d %d %d %d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].wt, p[i].tat);
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    read(0, &n, sizeof(int));
    read(0, p, sizeof(process)*n);

    for (int i = 0; i < n; i++)
        p[i].rt = p[i].bt;

    pthread_mutex_init(&lock, NULL);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, scheduler, NULL);
    pthread_create(&t2, NULL, display, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}