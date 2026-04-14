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
    Implement SCAN Disk Scheduling + Frequency-based Page Replacement (LFU-style)

    - Input:
        - Number of frames
        - Initial head position
        - Disk size
        - Pairs of (page number, cylinder position) until user enters -1
    - Task:
        - Apply SCAN Disk Scheduling (move right first) on cylinder positions
        - From the resulting order, generate the page reference sequence
        - Apply LFU Page Replacement (Lab 10) on this sequence
        - If a tie occurs in LFU, replace the page that appeared earlier in the sequence
    
    - Display:
        - Seek sequence
        - Total head movement
        - Page faults
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void sort(int arr[], int pages[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
                int p = pages[j]; pages[j] = pages[j+1]; pages[j+1] = p;
            }
        }
    }
}

int scan(int cyl[], int pages[], int n, int head, int size, int seq[]) {
    sort(cyl, pages, n);

    int total = 0, k = 0;
    int i;
    for(i = 0; i < n; ++i) {
        if(cyl[i] >= head) break;
    }

    printf("Seek Sequence: ");
    for(int j = i; j < n; ++j) {
        printf("%d ", cyl[j]);
        total += abs(head - cyl[j]);
        head = cyl[j];
        seq[k++] = pages[j];
    }

    if(head != size - 1) {
        total += abs(head - size + 1);
        head = size - 1;
    }

    for(int j = i - 1; j >= 0; --j) {
        printf("%d ", cyl[j]);
        total += abs(head - cyl[j]);
        head = cyl[j];
        seq[k++] = pages[j];
    }

    printf("\nTotal Head Movement: %d\n", total);
    return k;
}

void lfu(int pages[], int n, int cnt) {
    int frames[cnt], freq[cnt], time[cnt];
    int faults = 0, t = 0;

    for(int i = 0; i < cnt; ++i) {
        frames[i] = -1;
        freq[i] = 0;
        time[i] = 0;
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
            faults++;
            int idx =  -1;
            for(int j = 0; j < cnt; ++j) {
                if(frames[j] == -1) {
                    idx = j;
                    break;
                }
            }

            if(idx == -1) {
                idx = 0;
                for(int j = 1; j < cnt; ++j) {
                    if(freq[j] < freq[idx] || (freq[j] == freq[idx] && time[j] < time[idx])) idx = j;
                }
            }

            frames[idx] = pages[i];
            freq[idx] = 1;
            time[idx] = t++;
        }
    }
    printf("Page Faults: %d\n", faults);
}

int main() {
    int frames, head, size;

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter initial head: ");
    scanf("%d", &head);

    printf("Enter disk size: ");
    scanf("%d", &size);

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

    printf("\n--- SCAN Disk Scheduling ---\n");
    int len = scan(cyl, pages, n, head, size, seq);

    printf("Generated Page Sequence: ");
    for (int i = 0; i < len; i++)
        printf("%d ", seq[i]);
    printf("\n");

    printf("\n--- LFU Page Replacement ---\n");
    lfu(seq, len, frames);

    return 0;
}
