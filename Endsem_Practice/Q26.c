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
    Implement a program using:

    - Deadlock Detection 
    - LOOK Disk Scheduling 
    - FIFO Page Replacement
    
    - Input:
        - Allocation, Request, Available matrices
        - Number of frames
        - Initial head position
        - Pairs of (page number, cylinder position) until -1
    
    - Task:
        - Apply Deadlock Detection Algorithm

    - If deadlock exists:
        - Move disk head in left direction first (LOOK)

    - If no deadlock:
        - Move disk head in right direction first (LOOK)
        - From LOOK execution order:
        - Generate page reference sequence
        - Apply FIFO Page Replacement on generated sequence
    
    - Display:
        - Deadlock status
        - Seek sequence
        - Total head movement
        - Page faults
*/

#include <stdio.h>
#include <stdlib.h>

#define P 3
#define R 3
#define MAX 100

int allocation[P][R], request[P][R], available[R];

int detectDeadlock(int finish[]) {
    int work[R];

    for(int i = 0; i < R; ++i) {
        work[i] = available[i];
    }

    for(int i = 0; i < P; ++i) {
        int zero = 1;
        for(int j = 0; j < R; ++j) {
            if(allocation[i][j] != 0) zero = 0;
        }
        finish[i] = zero;
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

    for(int i = 0; i < P; ++i) {
        if(!finish[i]) return 1;
    }

    return 0;
}

void sort(int cyl[], int pages[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (cyl[j] > cyl[j+1]) {
                int t = cyl[j]; cyl[j] = cyl[j+1]; cyl[j+1] = t;
                int p = pages[j]; pages[j] = pages[j+1]; pages[j+1] = p;
            }
        }
    }
}

int look(int cyl[], int pages[], int n, int head, int dir, int seq[]) {
    sort(cyl, pages, n);

    int total = 0, k = 0;

    int i;
    for (i = 0; i < n; i++)
        if (cyl[i] >= head)
            break;

    printf("Seek Sequence: ");

    if(dir == 1) {
        for(int j = i; j < n; ++j) {
            printf("%d ", cyl[j]);
            total += abs(head - cyl[j]);
            head = cyl[j];
            seq[k++] = pages[j];
        }
        for (int j = i-1; j >= 0; j--) {
            printf("%d ", cyl[j]);
            total += abs(head - cyl[j]);
            head = cyl[j];
            seq[k++] = pages[j];
        }
    } else {
        for (int j = i-1; j >= 0; j--) {
            printf("%d ", cyl[j]);
            total += abs(head - cyl[j]);
            head = cyl[j];
            seq[k++] = pages[j];
        }
        for (int j = i; j < n; j++) {
            printf("%d ", cyl[j]);
            total += abs(head - cyl[j]);
            head = cyl[j];
            seq[k++] = pages[j];
        }
    }
    printf("\nTotal Head Movement: %d\n", total);
    return k;
}

int fifo(int pages[], int n, int frames) {
    int frame[frames], front = 0, faults = 0;

    for (int i = 0; i < frames; i++)
        frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < frames; j++)
            if (frame[j] == pages[i]) found = 1;

        if (!found) {
            frame[front] = pages[i];
            front = (front + 1) % frames;
            faults++;
        }
    }

    return faults;
}

int main() {
    int finish[P];

    printf("Enter Allocation:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &allocation[i][j]);

    printf("Enter Request:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &request[i][j]);

    printf("Enter Available:\n");
    for (int i = 0; i < R; i++)
        scanf("%d", &available[i]);

    int deadlock = detectDeadlock(finish);

    if (deadlock)
        printf("Deadlock Detected\n");
    else
        printf("No Deadlock\n");

    int frames, head;
    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter head: ");
    scanf("%d", &head);

    int pages[MAX], cyl[MAX], n = 0;

    printf("Enter (page, cylinder) pairs (-1 to stop):\n");

    while (1) {
        int p;
        scanf("%d", &p);

        if (p == -1) break;

        scanf("%d", &cyl[n]);
        pages[n] = p;
        n++;
    }

    int seq[MAX];

    int dir = deadlock ? 0 : 1; 

    int len = look(cyl, pages, n, head, dir, seq);

    printf("Page Sequence: ");
    for (int i = 0; i < len; i++)
        printf("%d ", seq[i]);
    printf("\n");

    int faults = fifo(seq, len, frames);

    printf("Page Faults (FIFO): %d\n", faults);

    return 0;
}