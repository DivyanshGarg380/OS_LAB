/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

/*  AGING Concept

    - Implement Preemptive Priority
    - If process waits too long → increase priority 
*/

// Parent.c is same as in Q20

// aging.c

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

typedef struct {
    int id, at, bt, rt, pr, ct, wt, tat, wait;
} process;

process p[10];
int n;

int time = 0, done = 0;
pthread_mutex_t lock;

void *schedular(void *arg) {
    while(done < n) {
        pthread_mutex_lock(&lock);
        int pos = -1, best = INT_MAX;

        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && p[i].rt > 0) {
                if(p[i].pr < best) {
                    best = p[i].pr;
                    pos = i;
                } 

                p[i].wait++;
                if(p[i].wait >= 3) {
                    p[i].pr--; // aging done here
                    p[i].wait = 0;
                }
            }
        }

        if(pos != -1) {
            printf("Time %d -> P%d\n", time_now, p[pos].id);

            p[pos].rt--;

            if (p[pos].rt == 0) {
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

