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
    Implement a program to:

    - Take page reference string
    - Perform FIFO and LRU Page Replacement
    - Compare page faults

    - If FIFO faults < LRU faults:
        - Perform FCFS Disk Scheduling 

    - Else:
        - Perform SSTF Disk Scheduling
        - Display total head movement
*/

#include <stdio.h>
#include <stdlib.h>

#define FRAMES 3
#define MAX 100

int fifo(int pages[], int n) {
    int frames[FRAMES], front = 0, faults = 0;

    for(int i = 0; i < FRAMES; ++i) frames[i] = -1;

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) found = 1;
        }

        if(!found) {
            frames[front] = pages[i];
            front = (front + 1) % FRAMES;
            faults++;
        }
    }
    return faults;
}

int lru(int pages[], int n) {
    int frames[FRAMES], time[FRAMES], faults = 0, cnt = 0;

    for(int i = 0; i < FRAMES; ++i) {
        frames[i] = -1;
        time[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) {
                found = 1;
                time[j] = ++cnt;
            }
        }

        if(!found) {
            int idx = 0;
            for(int j = 1; j < FRAMES; ++j) {
                if(time[j] < time[idx]) idx = j;
            }

            frames[idx] = pages[i];
            time[idx] = ++cnt;
            faults++;
        }
    }
    return faults;
}

void fcfs(int req[], int n, int head) {
    int total = 0;
    for(int i = 0; i < n; ++i) {
        total += abs(req[i] - head);
        head = req[i];
    }
    printf("FCFS Head movement: %d\n", total);
}

void sstf(int req[], int n, int head) {
    int visited[n];
    int total = 0;

    for(int i = 0; i < n; ++i) visited[i] = 0;

    for(int i = 0; i < n; ++i) {
        int mini = 1e9, idx = -1;
        for(int j = 0; j < n; ++j) {
            if(!visited[j]) {
                int dist = abs(head - req[j]);
                if(dist < mini) {
                    mini = dist;
                    idx = j;
                }
            }
        }

        visited[idx] = 1;
        total += mini;
        head = req[idx];
    }

    printf("SSTF head movement: %d\n", total);
}

int main() {
    int n;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    int f1 = fifo(pages, n);
    int f2 = lru(pages, n);

    printf("FIFO Faults = %d\n", f1);
    printf("LRU Faults = %d\n", f2);

    int d, head;

    printf("Enter number of disk requests: ");
    scanf("%d", &d);

    int req[d];
    printf("Enter requests: ");
    for (int i = 0; i < d; i++)
        scanf("%d", &req[i]);

    printf("Enter head: ");
    scanf("%d", &head);

    if (f1 < f2)
        fcfs(req, d, head);
    else
        sstf(req, d, head);

    return 0;
}