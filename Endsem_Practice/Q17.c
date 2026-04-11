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

    - Take segment sizes and memory limit
    - Check for segmentation validity (if any segment > limit → segmentation fault)

    - If segmentation fault occurs:
        - Perform First-Fit Allocation for given processes
        - Count allocated processes = A

    - If no segmentation fault:
        - Convert segments into pages
        - Take page reference string
        - Perform Optimal Page Replacement
        - Count page faults = F

    - Then:
        - If segmentation fault → use A disk requests
        - Else → use F disk requests
        - Take disk requests
        - Perform SCAN Disk Scheduling
        - Display total head movement
*/

#include <stdio.h>
#include <stdlib.h>

#define FRAMES 3
#define MAX 100

int firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n], cnt = 0;
    for(int i = 0; i < n; ++i) allocation[i] = -1;

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            if(blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                cnt++;
                break;
            }
        }
    }
    return cnt;
}

int findOptimal(int pages[], int frames[], int n, int idx) {
    int farthest = idx, pos = -1;

    for(int i = 0; i < FRAMES; ++i) {
        int j;
        for(j = idx; j < n; ++j) {
            if(frames[i] == pages[j]) {
                if(j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }

        if(j == n) return i;
    }

    return (pos == -1) ? 0 : pos;
}

int optimal(int pages[], int n) {
    int frames[FRAMES];
    int faults;

    for(int i = 0; i < FRAMES; ++i) frames[i] = -1;

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < FRAMES; ++j) {
            if(frames[j] == pages[i]) found = 1;
        }

        if(!found) {
            int pos = findOptimal(pages, frames, n, i + 1);
            frames[pos] = pages[i];
            faults++;
        }
    }

    return faults;
}

void sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1]) {
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
}

int scan(int req[], int n, int head, int size) {
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
    int s, limit;

    printf("Enter number of segments: ");
    scanf("%d", &s);

    int seg[s];
    printf("Enter segment sizes: ");
    for (int i = 0; i < s; i++)
        scanf("%d", &seg[i]);

    printf("Enter memory limit: ");
    scanf("%d", &limit);

    int faultFlag = 0;

    for (int i = 0; i < s; i++) {
        if (seg[i] > limit) {
            faultFlag = 1;
            break;
        }
    }

    int count = 0;

    if (faultFlag) {
        printf("Segmentation Fault Occurred\n");

        int m, n;
        printf("Enter number of blocks: ");
        scanf("%d", &m);

        int blockSize[m];
        for (int i = 0; i < m; i++)
            scanf("%d", &blockSize[i]);

        printf("Enter number of processes: ");
        scanf("%d", &n);

        int processSize[n];
        for (int i = 0; i < n; i++)
            scanf("%d", &processSize[i]);

        count = firstFit(blockSize, m, processSize, n);

    } else {
        printf("No Segmentation Fault\n");

        int n;
        printf("Enter number of pages: ");
        scanf("%d", &n);

        int pages[n];
        printf("Enter page reference string: ");
        for (int i = 0; i < n; i++)
            scanf("%d", &pages[i]);

        count = optimal(pages, n);
    }

    int req[MAX], head, size;

    printf("Enter disk size: ");
    scanf("%d", &size);

    printf("Enter head: ");
    scanf("%d", &head);

    printf("Enter %d disk requests: ", count);
    for (int i = 0; i < count; i++)
        scanf("%d", &req[i]);

    scan(req, count, head, size);

    return 0;
}
