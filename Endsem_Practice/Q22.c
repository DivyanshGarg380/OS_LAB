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
    - Allocate memory using Best-Fit 
    - Count number of allocated processes = A

    - Use A as:
        - Number of frames for paging
        - Take page reference string
        - Perform LRU Page Replacement 
        - Display page faults
*/

#include <stdio.h>

#define MAX 50

int bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    int cnt = 0;

    for(int i = 0; i < n; ++i) allocation[i] = -1;

    for(int i = 0; i < n; ++i) {
        int best = -1;
        for(int j = 0; j < m; ++j) {
            if(blockSize[j] >= processSize[i]){
                if(best == -1 || blockSize[j] < blockSize[best]) best = j;
            }
        }

        if(best != -1) {
            allocation[i] = best;
            blockSize[best] -= processSize[i];
            cnt++;
        }
    }

    printf("\nAllocated Process: %d\n", cnt);
    return cnt;
}

int lru(int pages[], int n, int frames) {
    int frame[frames], time[frames];
    int faults = 0, cnt = 0;

    for(int i = 0; i < frames; ++i) {
        frame[i] = -1;
        time[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < frames; ++j) {
            if(frame[j] == pages[i]) {
                found = 1;
                time[j] = ++cnt;
                break;
            }
        }

        if(!found) {
            int idx = 0;
            for(int j = 1; j < frames; ++j) {
                if(time[j] < time[idx]) idx = j;
            }

            frame[idx] = pages[i];
            time[idx] = ++cnt;
            faults++;
        }
    }
    return faults;
}


int main() {
    int m, n;

    printf("Enter number of blocks: ");
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

    int frames = bestFit(blockSize, m, processSize, n);

    if (frames == 0) {
        printf("No frames available\n");
        return 0;
    }

    int p;
    printf("\nEnter number of pages: ");
    scanf("%d", &p);

    int pages[p];
    printf("Enter pages: ");
    for (int i = 0; i < p; i++)
        scanf("%d", &pages[i]);

    int faults = lru(pages, p, frames);

    printf("Page Faults (LRU): %d\n", faults);

    return 0;
}
