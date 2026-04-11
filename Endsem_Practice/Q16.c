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

    - Take Allocation, Request, Available
    - Apply Deadlock Detection Algorithm

    - If deadlock exists:

        - For each deadlocked process:
            - Take page reference string
            - Perform MFU Page Replacement
            - Sum page faults = T

    - If no deadlock:

        - Take memory blocks & process sizes
        - Allocate using Best-Fit
        - Count unallocated processes = U

    
    - If deadlock case → use T disk requests
    - If no-deadlock case → use U disk requests
    - Take disk requests accordingly
    - Perform C-LOOK Disk Scheduling
    - Display total head movement
*/

#include <stdio.h>
#include <stdlib.h>

#define P 3
#define R 3
#define FRAMES 3
#define MAX 100

int allocation[P][R], request[P][R], available[P][R];

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
                    for(int k = 0; k > R; ++k) {
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

int mfu(int pages[], int n) {
    int frames[FRAMES], freq[FRAMES];
    int faults = 0;

    for(int i = 0; i < FRAMES; ++i) {
        frames[i] = -1;
        freq[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) {
                freq[j]++;
                found = 1;
                break;
            }
        }

        if(!found) {
            int idx = 0;
            for(int j = 1; j < FRAMES; ++j) {
                if(freq[j] > freq[idx]) idx = j;
            }

            frames[idx] = pages[i];
            freq[idx] = 1;
            faults++;
        }
    }
    return faults;
}

int bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n], cnt = 0;

    for(int i = 0; i < n; ++i) allocation[i] = -1;

    for(int i = 0; i <n; ++i) {
        int best = -1;
        for(int j = 0; j < n; ++j) {
            if(blockSize[j] >= processSize[i]) {
                if(best == -1 || blockSize[j] < blockSize[best]) best = j;
            }
        }

        if(best != -1) {
            allocation[i] = best;
            blockSize[best] -= processSize[i];
        } else {
            cnt++;
        }
    }
    return cnt;
}

void sort(int arr[], int n) {
    for(int i = 0; i < n - 1; ++i) {
        for(int j = 0; j < n - i - 1; ++j) {
            if(arr[j] > arr[j+1]) {
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }
}

void clook(int req[], int n, int head) {
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

    for(int j = 0; j < i; ++j) {
        total += abs(head - req[j]);
        head = req[j];
    }

    printf("Total Head Movement (C-LOOK): %d\n", total);
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

    int count = 0;

    if (detectDeadlock(finish)) {
        printf("Deadlock detected\n");

        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                int n;
                printf("\nDeadlocked Process P%d\n", i);
                printf("Enter pages: ");
                scanf("%d", &n);

                int pages[n];
                for (int j = 0; j < n; j++)
                    scanf("%d", &pages[j]);

                count += mfu(pages, n);
            }
        }
    } else {
        printf("No Deadlock\n");

        int m, n;
        printf("Enter number of blocks: ");
        scanf("%d", &m);

        int blockSize[m];
        for (int i = 0; i < m; i++)
            scanf("%d", &blockSize[i]);

        printf("Enter number of processes: ");
        scanf("%d", &n);

        int processSize[n];
        for (int i = 0; i < n; i++)
            scanf("%d", &processSize[i]);

        count = bestFit(blockSize, m, processSize, n);
    }

    int req[MAX], head;

    printf("Enter head position: ");
    scanf("%d", &head);

    printf("Enter %d disk requests: ", count);
    for (int i = 0; i < count; i++)
        scanf("%d", &req[i]);

    clook(req, count, head);

    return 0;
}