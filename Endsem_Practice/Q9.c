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
    - Take disk requests and initial head position
    - Perform C-LOOK Disk Scheduling Algorithm
    - Display the seek sequence and total head movement
    - Additionally, count how many requests were already “in memory” (simulate using a small frame array) and skip them
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 50
#define FRAMES 3

void sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
    
            }        
        }
    }
}

int inMemory(int frames[], int size, int val) {
    for(int i = 0; i < size; ++i) {
        if(frames[i] == val) return 1;
    }
    return 0;
}

void clook(int req[], int n, int head, int frames[]) {
    int total = 0;
    sort(req, n);
    int i;
    for(i = 0; i < n; ++i) {
        if(req[i] >= head) break;
    }

    printf("Seek Sequence: ");
    for(int j = i; j < n; ++j) {
        if(inMemory(frames, FRAMES, req[j])) continue;

        printf("%d ", req[j]);
        total += abs(head - req[j]);
        head = req[j];
    }

    for(int j = 0; i < i; ++i) {
        if(inMemory(frames, FRAMES, req[j])) continue;
        printf("%d ", req[j]);
        total += abs(head - req[j]);
        head = req[j];
    }
    printf("\nTotal Head Movement: %d\n", total);
}

int main() {
    int n, head;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    int req[n];
    printf("Enter requests: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    int frames[FRAMES];
    printf("Enter %d values already in memory: ", FRAMES);
    for (int i = 0; i < FRAMES; i++)
        scanf("%d", &frames[i]);

    clook(req, n, head, frames);

    return 0;
}