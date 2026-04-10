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
    - Allocate memory using First-Fit Allocation
    - Count number of successfully allocated processes = X

    - Use this X as:

    - Number of frames for FIFO Page Replacement
    - Then take page reference string and perform FIFO
    - Display page faults
*/

#include <stdio.h>

#define MAX 50

int firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    int cnt = 0;

    for(int i = 0; i < n; ++i) {
        allocation[i] = -1;
    }

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                cnt++;
                break;
            }
        }
    }

    printf("\nFirst Fit Allocation:\n");
    for (int i = 0; i < n; i++) {
        if (allocation[i] != -1)
            printf("P%d -> Block %d\n", i, allocation[i]);
        else
            printf("P%d -> Not Allocated\n", i);
    }

    printf("Allocated Processes = %d\n", cnt);

    return cnt;
}

void fifo(int pages[], int n, int cnt) {
    int frames[MAX];
    int front = 0, faults = 0;

    for(int i = 0; i < cnt; ++i) frames[i] = -1;

    for(int i = 0; i < n; ++i) {
        int fnd = 0;
        for(int j = 0; j < cnt; ++j) {
            if(frames[j] == pages[i]) {
                fnd = 1;
                break;
            }
        }

        if(!fnd) {
            frames[front] = pages[i];
            front = (front + 1) % cnt;
            faults++;
        }
    }
    printf("\nFIFO Page Faults (Frames = %d): %d\n", cnt, faults);
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

    int frames = firstFit(blockSize, m, processSize, n);

    if (frames == 0) {
        printf("No allocation → cannot proceed to paging\n");
        return 0;
    }

    int p;
    printf("\nEnter number of pages: ");
    scanf("%d", &p);

    int pages[p];
    printf("Enter page reference string: ");
    for (int i = 0; i < p; i++)
        scanf("%d", &pages[i]);

    fifo(pages, p, frames);

    return 0;
}