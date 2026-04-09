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
        - Take input for Allocation, Request, Available
        - Use Deadlock Detection Algorithm to determine if system is in deadlock
        - If deadlock exists → print deadlocked processes
        - If no deadlock:
            - For each process:
            - Take page reference string (user input)
            - Perform Optimal Page Replacement
            - Display page faults
*/

#include <stdio.h>

#define P 3
#define R 3
#define FRAMES 3

int allocation[P][R], request[P][R];
int available[R];

int detectDeadlock(int finish[]) {
    int work[R];
    for(int i = 0; i < R; ++i) work[i] = available[i];

    for(int i = 0; i < P; ++i) {
        int fnd = 1;
        for(int j = 0; j < R; ++j) {
            if(allocation[i][j] != 0) {
                fnd = 0;
                break;
            }
        }

        finish[i] = fnd;
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
                    for(int k = 0; k < R; ++k) work[k] += allocation[i][k];
                    finish[i] = 1;
                    changed = 1;
                }
            }
        }
    }

    int deadlock = 0;
    for (int i = 0; i < P; i++)
        if (!finish[i])
            deadlock = 1;

    return deadlock;
}

int findOptimal(int pages[], int frames[], int n, int index) {
    int farthest = index, pos = -1;

    for (int i = 0; i < FRAMES; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (j == n)
            return i;
    }

    return (pos == -1) ? 0 : pos;
}

void optimal(int pages[], int n) {
    int frames[FRAMES];
    int faults = 0;

    for (int i = 0; i < FRAMES; i++)
        frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < FRAMES; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if (!found) {
            int pos = findOptimal(pages, frames, n, i + 1);
            frames[pos] = pages[i];
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

    if (detectDeadlock(finish)) {
        printf("Deadlock detected!\nDeadlocked processes: ");
        for (int i = 0; i < P; i++)
            if (!finish[i])
                printf("P%d ", i);
        printf("\n");
        return 0;
    }

    printf("No Deadlock. Executing processes...\n");

    for (int i = 0; i < P; i++) {
        printf("\nProcess P%d\n", i);

        int n;
        printf("Enter number of pages: ");
        scanf("%d", &n);

        int pages[n];
        printf("Enter pages: ");
        for (int j = 0; j < n; j++)
            scanf("%d", &pages[j]);

        optimal(pages, n);
    }

    return 0;
}