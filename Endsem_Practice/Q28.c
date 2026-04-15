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
    Dynamic Memory Allocation + Safety Check
    - Input:
        - Number of processes n
        - Number of memory blocks m
        - Sizes of memory blocks
        - Maximum memory required for each process
        - Requests in form (Pid, memory) until -1
    
    - For each request:
        - Sort memory blocks in ascending order
        - Apply Best Fit Allocation
        - Display:
            - Memory before allocation
            - Memory after allocation
            - Update allocation for that process
            - Perform Safety Check (Banker logic):
                - If safe → print safe sequence
                - Else → rollback allocation
    
    - Output:
        - Allocation steps
        - Safe / Unsafe state
        - Safe sequence
        - Final allocation
*/

#include <stdio.h>

#define MAX 50

int block[MAX], maxP[MAX], alloc[MAX];
int n, m;

void sortBlocks() {
    for (int i = 0; i < m-1; i++) {
        for (int j = 0; j < m-i-1; j++) {
            if (block[j] > block[j+1]) {
                int t = block[j];
                block[j] = block[j+1];
                block[j+1] = t;
            }
        }
    }
}

int isSafe(int safeSeq[]) {
    int work = 0, finish[MAX] = {0}, cnt = 0;

    for(int i = 0; i < m; ++i) work += block[i];

    while(cnt < n) {
        int found = 0;
        for(int i = 0; i < n; ++i) {
            if(!finish[i]) {
                int need = maxP[i] - alloc[i];

                if(work >= need) {
                    work += alloc[i];
                    finish[i] = 1;
                    safeSeq[cnt++] = i;
                    found = 1;
                }
            }
        }
        if(!found) return 0;
    }
    return 1;
} 

int bestFit(int pid, int req) {
    int idx = -1;

    for(int i = 0; i < m; ++i) {
        if(block[i] >= req) {
            if(idx == -1 || block[i] < block[idx]) idx = i;
        }
    }

    return idx;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of blocks: ");
    scanf("%d", &m);

    printf("Enter block sizes: ");
    for (int i = 0; i < m; i++)
        scanf("%d", &block[i]);

    printf("Enter max need of each process:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &maxP[i]);
        alloc[i] = 0;
    }

    while(1) {
        int pid, req;

        printf("\nEnter PID: ");
        scanf("%d", &pid);

        if (pid == -1) break;

        scanf("%d", &req);

        printf("\nP%d request %d\n", pid, req);

        if(req > (maxP[pid] - alloc[pid])) {
            printf("Error: exceeds max need\n");
            continue;
        }

        sortBlocks();

        printf("Before: ");
        for (int i = 0; i < m; i++)
            printf("%d ", block[i]);
        printf("\n");

        int idx = bestFit(pid, req);

        if (idx == -1) {
            printf("Allocation not possible\n");
            continue;
        }

        block[idx] -= req;
        alloc[pid] += req;

        printf("After: ");
        for (int i = 0; i < m; i++)
            printf("%d ", block[i]);
        printf("\n");

        int safeSeq[MAX];

        if (isSafe(safeSeq)) {
            printf("Safe | Seq: ");
            for (int i = 0; i < n; i++)
                printf("P%d ", safeSeq[i]);
            printf("\n");
        } else {
            printf("Unsafe → Rolling back\n");
            block[idx] += req;
            alloc[pid] -= req;
        }
    }
    printf("\nFinal Allocation:\n");
    for (int i = 0; i < n; i++)
        printf("P%d: %d\n", i, alloc[i]);

    return 0;
}