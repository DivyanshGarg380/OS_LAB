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
    Take Allocation, Max, Available
    - Take a specific process request vector
    - Apply Resource Request Algorithm

    - If request can be granted safely:
        - Update system state
        - Take page reference string
        - Perform LRU Page Replacement
        - Count page faults = F

    - If request cannot be granted:
        - Perform Worst-Fit Allocation
        - Count unallocated processes = U

    - Then:
        - If request granted → use F disk requests
        - Else → use U disk requests
        - Take disk requests
        - Perform FCFS Disk Scheduling
        - Display total head movement
*/

#include <stdio.h>
#include <stdlib.h>

#define P 3
#define R 3
#define FRAMES 3
#define MAX 100

int allocation[P][R], max[P][R], need[P][R], available[R];

int isSafe() {
    int work[R], finish[P] = {0}, cnt = 0;
    
    for(int i = 0; i < R; ++i) work[i] = available[i];
    
    while(cnt < P) {
        int found = 0;
        for(int i = 0; i < P; ++i){
            if(!finish[i]) {
                int j;
                for(j = 0; j < R; ++j) {
                    if(need[i][j] > work[j]) break;
                }

                if(j == R) {
                    for(int k = 0; k < R; ++k) work[k] += allocation[i][k];

                    finish[i] = 1;
                    cnt++;
                    found = 1;
                }
            }
        }

        if(!found) return 0;
    }
    return 1;
}

int requestGrant(int pid, int req[]) {
    for(int i = 0; i < R; ++i) {
        if(req[i] > need[pid][i] || req[i] > available[i]) return 0;
    }

    for(int i = 0; i < R; ++i) {
        available[i] -= req[i];
        allocation[pid][i] += req[i];
        need[pid][i] -= req[i];
    }

    if(isSafe()) return 1;

    for(int i = 0; i < R; ++i) {
        available[i] += req[i];
        allocation[pid][i] -= req[i];
        need[pid][i] += req[i]; 
    }

    return 0;
}

int lru(int pages[], int n) {
    int frames[FRAMES], time[FRAMES];
    int faults = 0, cnt = 0;

    for(int i = 0; i < FRAMES; ++i) {
        frames[i] = -1;
        time[i] = 0;
    }

    for(int i = 0; i < n; ++i){
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
            for(int j = 1; j < FRAMES; ++j){
                if(time[j] < time[idx]) idx = j;
            }

            frames[idx] = pages[i];
            time[idx] = ++cnt;
            faults++;
        }
    }
    return faults;
}

int worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n], cnt = 0;
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
        } else {
            cnt++;
        }
    }

    return cnt;
}

void fcfs(int req[], int n, int head) {
    int total = 0;

    for (int i = 0; i < n; i++) {
        total += abs(head - req[i]);
        head = req[i];
    }

    printf("Total Head Movement (FCFS): %d\n", total);
}

int main() {
    int pid, req[R];

    printf("Enter Allocation:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &allocation[i][j]);

    printf("Enter Max:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Available:\n");
    for (int i = 0; i < R; i++)
        scanf("%d", &available[i]);

    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];

    printf("Enter process id: ");
    scanf("%d", &pid);

    printf("Enter request vector: ");
    for (int i = 0; i < R; i++)
        scanf("%d", &req[i]);

    int count = 0;

    if (requestGrant(pid, req)) {
        printf("Request Granted\n");

        int n;
        printf("Enter pages: ");
        scanf("%d", &n);

        int pages[n];
        for (int i = 0; i < n; i++)
            scanf("%d", &pages[i]);

        count = lru(pages, n);

    } else {
        printf("Request Denied\n");

        int m, n;
        printf("Enter blocks: ");
        scanf("%d", &m);

        int blockSize[m];
        for (int i = 0; i < m; i++)
            scanf("%d", &blockSize[i]);

        printf("Enter processes: ");
        scanf("%d", &n);

        int processSize[n];
        for (int i = 0; i < n; i++)
            scanf("%d", &processSize[i]);

        count = worstFit(blockSize, m, processSize, n);
    }

    int reqd[MAX], head;

    printf("Enter head: ");
    scanf("%d", &head);

    printf("Enter %d disk requests: ", count);
    for (int i = 0; i < count; i++)
        scanf("%d", &reqd[i]);

    fcfs(reqd, count, head);

    return 0;
}
