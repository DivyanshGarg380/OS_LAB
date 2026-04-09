/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

#include <stdio.h>
#include <stdlib.h>

#define P 3
#define R 3
#define FRAMES 3
#define MAX 100

int allocation[P][R], request[P][R], available[R];

int detectDeadlock(int finish[]) {
    int work[R];

    for(int i = 0; i < R; ++i) work[i] = available[i];

    for(int i = 0; i < P; ++i) {
        int zero = 1;
        for(int j = 0; j < R; ++j) {
            if(allocation[i][j] != 0) zero = 0;
        }
        finish[i] = zero;
    }

    int changed = 1;
    while(changed) {
        changed = 0;
        for(int i = 0; i < P; ++i) {
            if(!finish[i]) {
                int j;
                for(j = 0; j < R; ++j) {
                    if(request[i][j] > work[j]) break;
                }

                if(j == R) {
                    for(int k = 0; k < R; ++k) {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = 1;
                    changed = 1;
                }
            }
        }
    }

    for(int i = 0; i < P; ++i) {
        if(!finish[i]) return 1;
    }

    return 0;
}

void sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1]) {
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
}

void cscan(int req[], int n, int head, int size) {
    int total = 0;
    sort(req, n);
    int i;
    for(i = 0; i < n; ++i) {
        if(req[i] >= head) break;
    }

    for(int j = i; j < n; ++j) {
        total += abs(head - req[j]);
        head = req[j];
    }

    total += abs(head - size + 1);
    head = 0;
    total += size - 1;

    for(int j = 0; j < i; ++j) {
        total += abs(head - req[j]);
        head = req[j];
    }

    printf("Total Head Movement: %d\n", total);
}

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
            if(frames[j] == pages[i]) fnd = 1;
        }

        if(!fnd) {
            int pos = findOptimal(pages, frames, n, i + 1);
            frames[pos] = pages[i];
            faults++;
        }
    }

    printf("Page Faults: %d\n", faults);
}

int main() {
    int finish[P];

    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &allocation[i][j]);

    printf("Enter Request Matrix:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &request[i][j]);

    printf("Enter Available:\n");
    for (int i = 0; i < R; i++)
        scanf("%d", &available[i]);

    if (detectDeadlock(finish)) {
        printf("Deadlock detected!\n");
        return 0;
    }

    printf("No Deadlock.\n");

    int n, head, size;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    int req[n];
    printf("Enter requests: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter head position: ");
    scanf("%d", &head);

    printf("Enter disk size: ");
    scanf("%d", &size);

    cscan(req, n, head, size);

    printf("\nEnter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    optimal(pages, n);

    return 0;
}