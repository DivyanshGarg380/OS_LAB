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
    MRU Page Replacement using Reference Registers

    - Design and implement a program to simulate the Most Recently Used (MRU) page replacement algorithm using 8-bit reference registers.
    - Requirements:
        - Each frame has an 8-bit register.
        - On each page reference:
        - Shift all registers right by 1 bit.
        - Set MSB = 1 for the referenced page.
        - If page fault occurs and no free frame:
        - Replace the page with the highest register value (MRU).
        - After each reference:
        - Display frame contents with 8-bit registers.
        - Indicate "Hit" or "Fault".
        - At the end:
        - Display total hits and faults.

    - Output Format:
        - After each page:
        - frame contents with register values
        - Hit / Fault

    - Final Output:
        - Total Hits
        - Total Faults
*/

#include <stdio.h>

#define FRAMES 3
#define MAX 40

int main() {
    int pages[MAX], n;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter page string:\n");
    for(int i = 0; i < n; ++i) {
        scanf("%d", &pages[i]);
    }

    int frame[FRAMES];
    int reg[FRAMES];

    for(int i = 0; i < FRAMES; ++i) {
        frame[i] = -1;
        reg[i] = 0;
    }

    int hits = 0, faults = 0;

    for(int i = 0; i < n; ++i) {
        int page = pages[i];
        int found = -1;

        for(int j = 0; j < FRAMES; ++j) reg[j] = reg[j] >> 1;

        for(int j = 0; j < FRAMES; ++j) {
            if(frame[j] == page) {
                found = j;
                break;
            }
        }

        if(found != -1) {
            hits++;
            reg[found] |= 128;
            printf("\nPage %d: HIT\n", page);
        } else {
            faults++;
            int pos = -1;

            for(int j = 0; j < FRAMES; ++j) {
                if(frame[j] == -1) {
                    pos = j;
                    break;
                }
            }

            if(pos == -1) {
                int maxi = reg[0];
                pos = 0;

                for(int j = 1; j < FRAMES; ++j) {
                    if(reg[j] > maxi) {
                        maxi = reg[j];
                        pos = j;
                    }
                }
            }

            frame[pos] = page;
            reg[pos] |= 128;
            printf("\nPage %d: FAULT\n", page);
        }
        printf("Frames:\n");
        for(int j = 0; j < FRAMES; j++) {
            if(frame[j] != -1)
                printf("Page: %d | Reg: %03d\n", frame[j], reg[j]);
            else
                printf("Page: - | Reg: %03d\n", reg[j]);
        }
    }
    printf("\nTotal Hits = %d\n", hits);
    printf("Total Faults = %d\n", faults);

    return 0;
}


