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
    Implement a multi-threaded system where each thread (process) requests resources.
    Use Banker’s Algorithm to check safe state before execution.
    If safe → allow execution inside mutex critical section.
    Inside critical section, perform FIFO page replacement for given page sequence.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define P 3
#define R 3
#define FRAMES 3

int alloc[P][R], max[P][R], need[P][R];
int available[R];

pthread_mutex_t lock;

int isSafe() {
    int work[R], finish[P] = {0}, cnt = 0;

    for(int i = 0; i < R; ++i) work[i] = available[i];
    while(cnt < P) {
        int fnd = 0;
        for(int i = 0; i < P; ++i) {
            if(!finish[i]) {
                int j;
                for(j = 0; j < R; ++j) {
                    if(need[i][j] > work[j]) break;
                }

                if(j == R) {
                    for(int k = 0; k < R; ++k) {
                        work[k] += allocation[i][k];
                    }

                    finish[i] = 1;
                    fnd = 1;
                    cnt++;
                }
            }
        }
        if(!fnd) return 0;
    }
    return 1;
}

void fifo(int pages[], int n) {
    int frames[FRAMES], front = 0, faults = 0;

    for(int i = 0; i < FRAMES; ++i) frames[i] = -1;

    for(int i = 0; i < n; ++i) {
        int fnd = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) {
                fnd = 1;
                break;
            }
        }

        if(!fnd) {
            frames[front] = pages[i];
            front = (front + 1) % FRAMES;
            faults++;
        }

        printf("Frames: ");
        for(int j = 0; j < FRAMES; ++j) printf("%d ", frames[j]);
        printf("\n");
    }
    printf("Page Faults: %d\n", faults);
}

void* process(void *arg) {
    int id = *(int*) arg;
    pthread_mutex_lock(&lock);
    printf("\nProcess %d executing\n", id);

    int n;
    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);

    fifo(pages, n);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t[P];
    int id[P];

    pthread_mutex_init(&lock, NULL);

    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &allocation[i][j]);

    printf("Enter Max Matrix:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Available:\n");
    for (int i = 0; i < R; i++)
        scanf("%d", &available[i]);

    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];

    if (!isSafe()) {
        printf("Unsafe state!\n");
        return 0;
    }

    printf("Safe state. Executing...\n");

    for (int i = 0; i < P; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, process, &id[i]);
    }

    for (int i = 0; i < P; i++) pthread_join(t[i], NULL);

    pthread_mutex_destroy(&lock);
    return 0;
    
}