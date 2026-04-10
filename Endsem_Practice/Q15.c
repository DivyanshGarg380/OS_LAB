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

    - Take Allocation, Max, Available
    - Apply Banker’s Algorithm 
    - If system is safe:
        - For each process in safe sequence:
            - Take page reference string
    - Perform LFU Page Replacement
    - Count page faults
    - Compute total page faults = T

    - Use T as:
        - Number of disk requests
        - Take T disk requests
        - Perform LOOK Disk Scheduling
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
    int work[R], finish[P] = {0}, cnt =  0;

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
                    for(int k = 0; k < R; ++k) work[k] += allocation[i][k];

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

int lfu(int pages[], int n) {
    int frames[FRAMES], freq[FRAMES];
    int faults = 0;

    for(int i = 0; i < FRAMES; ++i) {
        frames[i] = -1;
        freq[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        int fnd = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) {
                freq[j]++;
                fnd = 1;
                break;
            }
        }

        if(!fnd) {
            int idx = 0;
            for(int j = 1; j < FRAMES; ++j) {
                if(freq[j] < freq[idx]) idx = j;
            }

            frames[idx] = pages[i];
            freq[idx] = 1;
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

void look(int req[], int n, int head) {
    int total = 0;
    sort(req, n);

    int i;
    for(i = 0; i < n; ++i) {
        if(req[i] >= head) break;
    }

    for(int j = i; j < n; ++j) {
        total += abs(req[j] - head);
        head = req[j];
    }

    for(int j = i - 1; j >= 0; --j) {
        total += abs(req[j] - head);
        head = req[j];
    }

    printf("Total Head Movement (LOOK): %d\n", total);
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

        totalFaults += lfu(pages, n);
    }

    printf("\nTotal Page Faults = %d\n", totalFaults);

    int req[MAX], head;

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter %d disk requests: ", totalFaults);
    for (int i = 0; i < totalFaults; i++)
        scanf("%d", &req[i]);

    look(req, totalFaults, head);

    return 0;
}