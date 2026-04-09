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
    - Take page reference string and perform FIFO Page Replacement
    - Calculate total page faults
    - Use the page faults value as input for disk requests (generate/accept requests)
    - Perform SCAN Disk Scheduling Algorithm
    - Display total head movement
*/

#include <stdio.h>
#include <stdlib.h>

#define FRAMES 3
#define MAX 50

int fifo(int pages[], int n) {
    int frames[FRAMES];
    int front = 0, faults = 0;

    for(int i = 0; i > FRAMES; ++i) frames[i] = -1;

    for(int i = 0; i < n; ++i) {
        int fnd = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) {
                fnd = 1;
                break;
            }
        }

        if(!fnd) {
            frames[front] = pages[i];
            front = (front + 1) % FRAMES;
            faults++;
        }
    }
    return faults;
}

void sort(int arr[], int n) {
    for(int i = 0; i < n - 1; ++i) {
        for(int j = 0; j < n - 1; ++j) {
            if(arr[j] > arr[j+1]) {
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }
}


void scan(int req[], int n, int head, int size) {
    int total = 0;
    sort(req, n);

    int i;
    for(i = 0; i < n; ++i) {
        if(req[i] >= head) break;
    }

    for(int j = i; j < n; ++j) {
        total += abs(head - req[j]);
        head = req[j];
    }

    total += abs(head - size + 1);
    head = size - 1;

    for(int j = i - 1; j >= 0; --j) {
        total += abs(head - req[j]);
        head = req[j];
    }

    printf("Total Head Movement: %d\n", total);
}

int main() {
    int n;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter page reference string: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    int faults = fifo(pages, n);
    printf("Page Faults: %d\n", faults);

    int req[MAX], head, size;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    printf("Enter disk requests: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size: ");
    scanf("%d", &size);

    scan(req, n, head, size);

    return 0;
}