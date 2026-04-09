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
    Each process takes a page reference string (user input) and performs Optimal Page Replacement.
    Display frame status after each step and total page faults.
*/

#include <stdio.h>
#include <pthread.h>

#define FRAMES 3
#define P 3

int findOptimal(int pages[], int frames[], int n, int idx) {
    int farthest = idx, pos = -1;

    for(int i = 0; i < FRAMES; ++i) {
        int j;
        for(j = idx; j < n; ++j) {
            if(frames[i] == pages[j]) {
                if(j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }

        if(j == n) return i;
    }
    return (pos == -1) ? 0 : pos;
}

void optimal(int pages[], int n) {
    int frames[FRAMES];
    int faults = 0;

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
            int pos = findOptimal(pages, frames, n, i + 1);
            frames[pos] = pages[i];
            faults++;
        }

        printf("Frames: ");
        for (int j = 0; j < FRAMES; j++)
            printf("%d ", frames[j]);
        printf("\n");
    }
    printf("Page Faults: %d\n", faults);
}

void* process(void* arg) {
    int id = *(int*)arg;

    pthread_mutex_lock(&lock);

    printf("\nProcess %d\n", id);

    int n;
    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    optimal(pages, n);

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