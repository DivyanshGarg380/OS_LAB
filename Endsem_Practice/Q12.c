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

    - Take memory block sizes and process sizes
    - Allocate memory using Best-Fit Allocation
    - Display allocation result
    - Then take page reference string and perform LFU Page Replacement
    - Display page faults
*/

#include <stdio.h>

#define MAX 50
#define FRAMES 3

void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];

    for(int i = 0; i < n; ++i) allocation[i] = -1;

    for(int i = 0; i < n; ++i) {
        int best = -1;
        for(int j = 0; j < m; ++j) {
            if(blockSize[j] >= processSize[i]) {
                if(best == -1 || blockSize[j] < blockSize[best]) best = j;
            }
        }

        if(best != -1) {
            allocation[i] = best;
            blockSize[best] -= processSize[i];
        }
    }

    printf("\nBest Fit Allocation:\n");
    for (int i = 0; i < n; i++) {
        if (allocation[i] != -1)
            printf("Process %d -> Block %d\n", i, allocation[i]);
        else
            printf("Process %d -> Not Allocated\n", i);
    }
}

void lfu(int pages[], int n){
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

    printf("\nPage Faults (LFU): %d\n", faults);
}

int main() {
    int m, n;

    printf("Enter number of memory blocks: ");
    scanf("%d", &m);

    int blockSize[m];
    printf("Enter block sizes: ");
    for (int i = 0; i < m; i++)
        scanf("%d", &blockSize[i]);

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int processSize[n];
    printf("Enter process sizes: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &processSize[i]);

    bestFit(blockSize, m, processSize, n);

    int p;
    printf("\nEnter number of pages: ");
    scanf("%d", &p);

    int pages[p];
    printf("Enter page reference string: ");
    for (int i = 0; i < p; i++)
        scanf("%d", &pages[i]);

    lfu(pages, p);

    return 0;
}