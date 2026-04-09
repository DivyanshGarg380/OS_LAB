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
    Implement a multi-threaded program where each thread represents a process.
    Use mutex for synchronization.
    Each process generates a page reference string (user input) and performs LRU Page Replacement.
    Display frame status and total page faults for each process.
*/

#include <stdio.h>
#include <pthread.h>

#define FRAMES 3
#define P 3

pthread_mutex_t lock;

void lru(int pages[], int n) {
    int frames[FRAMES], time[FRAMES];
    int faults = 0, cnt = 0;

    for(int i = 0; i < FRAMES; ++i){
        frames[i] = -1;
        time[i] = 0;
    }

    for(int i = 0; i < n; ++i){
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

            frame[idx] = pages[i];
            time[idx] = ++cnt;
            faults++;
        }
        printf("Frames: ");
        for (int j = 0; j < FRAMES; j++)
            printf("%d ", frames[j]);
        printf("\n");
    }
    printf("Page Faults: %d\n", faults);
}

void* process(void *arg) {
    int id = *(int*) arg;
    pthread_mutex_lock(&lock);

    printf("\nProcess %d\n", id);

    int n;
    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    lru(pages, n);

    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main() {
    pthread_t t[P];
    int id[P];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < P; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, process, &id[i]);
    }

    for (int i = 0; i < P; i++)
        pthread_join(t[i], NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}