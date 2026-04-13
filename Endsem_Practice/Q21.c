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
    Implement LCFS (Last Come First Serve) Disk Scheduling along with LFU (Least Frequently Used) Page Replacement.

    - Input:
        - Number of frames
        - Initial head position
        - Pairs of (page number, cylinder position) until user enters -1
    
    - Task:
        - Apply LCFS Disk Scheduling on the given requests
        - Based on the execution order, generate the page reference sequence
        - Apply LFU Page Replacement on this sequence
    
    - Display:
        - Seek sequence
        - Total head movement
        - Page faults
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int lcfs(int cyl[], int pages[], int n, int head, int seq[]) {
    int total = 0;
    
    printf("Seek Sequence: ");
    for(int i = n - 1; i >= 0; --i) {
        printf("%d ", cyl[i]);
        total += abs(head - cyl[i]);
        head = cyl[i];

        seq[n - i - 1] = pages[i];
    }

    printf("Total head movement: %d\n", total);
    return total;
}

void lfu(int pages[], int n, int cnt) {
    int frames[cnt], freq[cnt], faults = 0;

    for(int i = 0; i < cnt; ++i) {
        frames[i] = -1;
        freq[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        int fnd = 0;
        for(int j = 0; j < cnt; ++j) {
            if(frames[j] == pages[i]) {
                freq[j]++;
                fnd = 1;
                break;
            }
        }

        if(!fnd) {
            int idx = 0;
            for(int j = 1; j < cnt; ++j) {
                if(freq[j] < freq[idx]) idx = j;
            }

            frames[idx] = pages[i];
            faults++;
            freq[idx]  = 1;
        }
    }
    printf("Page faults: %d\n", faults);
}

int main() {
    int frames, head;

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    int pages[MAX], cyl[MAX], n = 0;

    printf("Enter (page, cylinder) pairs (-1 to stop):\n");

    while (1) {
        int p;
        scanf("%d", &p);

        if (p == -1)
            break;

        scanf("%d", &cyl[n]);
        pages[n] = p;
        n++;
    }

    int seq[n];

    printf("\n--- LCFS Disk Scheduling ---\n");
    lcfs(cyl, pages, n, head, seq);

    printf("Generated Page Sequence: ");
    for (int i = 0; i < n; i++)
        printf("%d ", seq[i]);
    printf("\n");

    printf("\n--- LFU Page Replacement ---\n");
    lfu(seq, n, frames);

    return 0;
}
