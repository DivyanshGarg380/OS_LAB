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
    Implement LSTF (Largest Seek Time First) Disk Scheduling along with MFU (Most Frequently Used) Page Replacement.

    - Input:
        - Number of frames
        - Initial head position
        - Pairs of (page number, cylinder position) until user enters -1
    
    - Task:
        - Apply LSTF Disk Scheduling on the given cylinder requests
        - Based on the execution order, generate the page reference sequence
        - Apply MFU Page Replacement on this sequence
        - Display:
            - Seek sequence
            - Total head movement
            - Page faults
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int lstf(int cyl[], int pages[], int n, int head, int seq[]) {
    int visited[n];
    int total = 0;

    for(int i = 0; i < n; ++i) visited[i] = 0;

    printf("Seek Sequence: ");
    for(int i = 0; i < n; ++i){
        int maxi = -1, idx = -1;
        for(int j = 0; j < n; ++j) {
            if(!visited[j]) {
                int dist = abs(head - cyl[j]);
                if(dist > maxi) {
                    maxi = dist;
                    idx = j;
                }
            }
        }

        visited[idx] = 1;
        total += maxi;
        head = cyl[idx];

        printf("%d ", cyl[idx]);
        seq[i] = pages[idx];
    }

    printf("Total head movement: %d\n", total);
    return total;
}  

void mfu(int pages[], int n, int cnt) {
    int frames[cnt], freq[cnt];
    int faults = 0;

    for(int i = 0; i < cnt; ++i) {
        frames[i] = -1;
        freq[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < cnt; ++j) {
            if(frames[j] == pages[i]) {
                freq[j]++;
                found = 1;
                break;
            }
        }

        if(!found) {
            int idx = 0;
            for(int j = 1; j < cnt; ++j) {
                if(freq[j] > freq[idx]) idx = j;
            }

            frames[idx] = pages[i];
            freq[idx] = 1;
            faults++;
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

    printf("\n--- LSTF Disk Scheduling ---\n");
    lstf(cyl, pages, n, head, seq);

    printf("Generated Page Sequence: ");
    for (int i = 0; i < n; i++)
        printf("%d ", seq[i]);
    printf("\n");

    printf("\n--- MFU Page Replacement ---\n");
    mfu(seq, n, frames);

    return 0;
}