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
    Enhanced LRU with Aging Counter

    - Design a program to simulate LRU Page Replacement using an aging mechanism.

    - Rules:
        - Each frame has a counter (integer)
        - Initially all counters = 0

    - For every page reference:
        - Increment all counters by 1
        - If page is present:
        - Set its counter = 0 (most recently used)
        - If page fault:
            - If empty frame → insert page, counter = 0
            - Else → replace page with highest counter value (least recently used)
    
    - Output:
        - After each step:
        - Frame contents
        - Counter values
        - Hit / Fault

    - Final:
        - Total Hits
        - Total Faults
*/

#include <stdio.h>

int main() {
    int frames, n;

    printf("Enter number of frames: ");
    scanf("%d", &n);

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    int frame[frames], counter[frames];
    int hits = 0, faults = 0;

    for(int i = 0; i < frames; ++i) {
        frame[i] = -1;
        counter[i] = 0;
    }

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < frames; ++j) {
            counter[j]++;
        }

        int found = 0;
        for(int j = 0; j < frames; ++j) {
            if(frame[j] == pages[i]) {
                found = 1;
                counter[j] = 0;
                hits++;
                break;
            }
        }

        if(!found) {
            faults++;
            int idx = -1;
            for(int j = 0; j < frames; ++j) {
                if(frame[j] == -1) {
                    idx = j;
                    break;
                }
            }

            if(idx == -1) {
                idx = 0;
                for(int j = 1; j < frames; ++j) {
                    if(counter[j] > counter[idx]) idx = j;
                }
            }

            frame[idx] = pages[i];
            counter[idx] = 0;
        }

        printf("\nAfter %d:\n", pages[i]);
        for (int j = 0; j < frames; j++)
            printf("F%d: %d (C=%d)\n", j, frame[j], counter[j]);

        if (found) printf("Hit\n");
        else printf("Fault\n");
    }

    printf("\nTotal Hits = %d\n", hits);
    printf("Total Faults = %d\n", faults);

    return 0;
}