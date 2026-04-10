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
    - Allocate memory using Worst-Fit Allocation 
    - Calculate total unused memory (internal fragmentation) = F

    - Use F as:

    - Initial disk head position
    - Take disk requests
    - Perform SSTF Disk Scheduling 
    - Display total head movement
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 50

int worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    int total = 0;

    for(int i = 0; i < n; ++i) allocation[i] = -1;

    for(int i = 0; i < n; ++i) {
        int worst = -1;
        for(int j = 0; j < m; ++j) {
            if(blockSize[j] >= processSize[i]) {
                if(worst == -1 || blockSize[j] > blockSize[worst]) worst = j;
            }
        }

        if(worst != -1) {
            allocation[i] = worst;
            blockSize[worst] -= processSize[i];
        }
    }

    printf("\nWorst Fit Allocation:\n");
    for (int i = 0; i < n; i++) {
        if (allocation[i] != -1)
            printf("P%d -> Block %d\n", i, allocation[i]);
        else
            printf("P%d -> Not Allocated\n", i);
    }

    for (int i = 0; i < m; i++)
        total += blockSize[i];

    printf("Total Unused Memory = %d\n", total);

    return total;
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
                    idx= j;
                }
            }
        }

        visited[idx] = 1;
        total += mini;
        head = req[idx];
    }
    printf("Total Head Movement (SSTF): %d\n", total);
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

    int head = worstFit(blockSize, m, processSize, n);

    int d;
    printf("\nEnter number of disk requests: ");
    scanf("%d", &d);

    int req[d];
    printf("Enter disk requests: ");
    for (int i = 0; i < d; i++)
        scanf("%d", &req[i]);

    sstf(req, d, head);

    return 0;
}