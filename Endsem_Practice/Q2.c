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
    Implement a system where multiple threads request resources dynamically at runtime.
    Use Banker’s Algorithm for every request (not just initial check).
    If unsafe → block the thread using semaphore.
    If safe → allow execution inside mutex critical section.
    Inside critical section, perform LRU page replacement and display page faults.
*/

#include <stdio.h>
#include <semaphore.h>

#define P 3
#define R 3
#define FRAMES 3

int allocations[P][R], max[P][R], need[P][R];
int available[R];

sem_t sem;

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
                    for(int k = 0; k < R; ++k) work[k] += allocations[i][k];
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

void lru(int pages[], int n) {
    int frames[FRAMES], time[FRAMES];
    int faults = 0, cnt = 0;

    for(int i = 0; i < FRAMES; ++i) {
        frames[i] = -1;
        times[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        int fnd = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) {
                fnd = 1;
                time[j] = ++cnt;
                break;
            }
        }

        if(!fnd) {
            int idx = 0;
            for(int j = 1; j < FRAMES; ++j) {
                if(time[j] < time[idx]) idx = j;
            }

            frames[idx] = pages[i];
            time[idx] = ++cnt;
            faults++;
        }

        printf("Frames: ");
        for (int j = 0; j < FRAMES; j++) printf("%d ", frames[j]);
        printf("\n");
    }
    printf("Page Faults: %d\n", faults);
}

void* process(void *arg) {
    int id = *(int*)arg;

    int request[R];
    printf("\nEnter resource request for P%d: ", id);
    for (int i = 0; i < R; i++) scanf("%d", &request[i]);

    sem_wait(&sem);
    for(int i = 0; i < R; ++i) {
        available[i] -= request[i];
        allocations[id][i] += request[i];
        need[id][i] -= request[i];
    }

    if(!isSafe()) {
        printf("P%d request denied (unsafe)\n", id);

        for (int i = 0; i < R; i++) {
            available[i] += request[i];
            allocation[id][i] -= request[i];
            need[id][i] += request[i];
        }

        sem_post(&sem);
        pthread_exit(NULL);
    }

    printf("P%d executing (safe)\n", id);

    int n;
    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);

    lru(pages, n);

    sem_post(&sem);

    pthread_exit(NULL);
}

int main() {
    pthread_t t[P];
    int id[P];

    sem_init(&sem, 0, 1);

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

    for (int i = 0; i < P; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, process, &id[i]);
    }

    for (int i = 0; i < P; i++)
        pthread_join(t[i], NULL);

    sem_destroy(&sem);

    return 0;
}