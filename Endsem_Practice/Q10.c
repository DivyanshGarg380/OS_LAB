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

    - Take input for Allocation, Max, Available
    - Apply Banker’s Algorithm to find safe sequence
    - If system is safe:
        - Execute processes in safe order
        - For each process:
            - Take page reference string
            - Perform LRU Page Replacement
            - Count page faults
    - After all processes execute:
        - Take disk requests (number = total page faults from all processes)
        - Perform SCAN Disk Scheduling
        - Display total head movement
*/

#include <stdio.h>
#include <stdlib.h>

#define P 3
#define R 3
#define FRAMES 3
#define MAX 100

int allocation[P][R], max[P][R], need[P][R];
int available[R];

int isSafe(int safeSeq[]) {
    int work[R], finish[P] = {0}, cnt = 0;
    for(int i = 0; i < R; ++i) {
        work[i] = available[i];
    }

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

                    safeSeq[cnt++] = i;
                    finish[i] = 1;
                    fnd = 1;
                }
            }
        }
        if(!fnd) return 0;
    }
    return 1;
}

int lru(int pages[], int n) {
    int frames[FRAMES], time[FRAMES];
    int faults = 0, cnt = 0;

    for(int i = 0; i < FRAMES; ++i){
        frames[i] = -1;
        time[i] = 0;
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
    }
    return faults;
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

void scan(int req[], int n, int head, int size) {
    int total = 0;
    sort(req,n);

    int i;
    for(i = 0; i < n; ++i) {
        if(req[i] >= head) break;
    }

    for(int j = i; j < n; ++j) {
        total += abs(head - req[j]);
        head = req[j];
    }

    total += abs(head - size + 1);
    head = size - 1;
    for(int j = i - 1; j >= 0; --j) {
        total += abs(head - req[j]);
        head = req[j];
    }

    printf("Total Head Movement: %d\n", total);
}

int main() {
    int safeSeq[P];

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

    if (!isSafe(safeSeq)) {
        printf("System NOT safe\n");
        return 0;
    }

    printf("Safe Sequence: ");
    for (int i = 0; i < P; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    int totalFaults = 0;

    for (int i = 0; i < P; i++) {
        int n;
        printf("\nProcess P%d\n", safeSeq[i]);
        printf("Enter number of pages: ");
        scanf("%d", &n);

        int pages[n];
        printf("Enter pages: ");
        for (int j = 0; j < n; j++)
            scanf("%d", &pages[j]);

        totalFaults += lru(pages, n);
    }

    printf("\nTotal Page Faults: %d\n", totalFaults);

    int req[MAX], head, size;

    printf("Enter disk size: ");
    scanf("%d", &size);

    printf("Enter initial head: ");
    scanf("%d", &head);

    printf("Enter %d disk requests: ", totalFaults);
    for (int i = 0; i < totalFaults; i++)
        scanf("%d", &req[i]);

    scan(req, totalFaults, head, size);

    return 0;
}