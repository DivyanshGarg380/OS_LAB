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
    - Use Banker’s Algorithm to find safe sequence
    - If safe:
        - For each process in safe sequence:
            - Take page reference string (user input)
            - Perform LRU Page Replacement
            - Display page faults
*/

#include <stdio.h>

#define P 3
#define R 3
#define FRAMES 3

int allocation[P][R], max[P][R], need[P][R];
int available[R];

int isSafe(int safeSeq[]) {
    int work[R], finish[P] = {0}, cnt = 0;

    for(int i = 0; i < R; ++i) work[i] = available[i];

    while(cnt < P){
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


void lru(int pages[], int n){
    int frames[FRAMES], time[FRAMES];
    int faults = 0, cnt = 0;

    for (int i = 0; i < FRAMES; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < FRAMES; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                time[j] = ++cnt;
                break;
            }
        }

        if (!found) {
            int idx = 0;
            for (int j = 1; j < FRAMES; j++)
                if (time[j] < time[idx]) idx = j;

            frames[idx] = pages[i];
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
        printf("System is NOT safe\n");
        return 0;
    }

    printf("Safe Sequence: ");
    for (int i = 0; i < P; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    for (int i = 0; i < P; i++) {
        int p = safeSeq[i];

        printf("\nProcess P%d\n", p);

        int n;
        printf("Enter number of pages: ");
        scanf("%d", &n);

        int pages[n];
        printf("Enter pages: ");
        for (int j = 0; j < n; j++)
            scanf("%d", &pages[j]);

        lru(pages, n);
    }

    return 0;
}