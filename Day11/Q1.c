#include <stdio.h>
#include <stdlib.h>

void fcfs(int req[], int n, int head) {
    int total = 0;
    for(int i = 0; i < n; ++i) {
        total += abs(req[i] - head);
        head = req[i];
    }

    printf("FCFS Head movement: %d\n", total);
}

void sstf(int req[], int n, int head) {
    int visited[n];

    for(int i = 0; i < n; ++i) visited[i] = 0;
    int total = 0;

    for(int i = 0; i < n; ++i) {
        int mini = 1e4, idx = -1;
        for(int j = 0; j < n; ++j) {
            if(!visited[j]) {
                int dist = abs(req[j] - head);
                if(dist < mini) {
                    mini = dist;
                    idx = j;
                }
            }
        }

        visited[idx] = 1;
        total += mini;
        head = req[idx];
    }

    printf("SSTF total head movement: %d\n", total);
}

void scan(int req[], int n, int head, int size) {
    int total = 0;
    for(int i = head; i >= 0; --i) {
        for(int j = 0; j < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        }
    }

    for(int i = head; i < size; ++i) {
        for(int j = 0; j < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        }
    }

    printf("SCAN total head movement: %d\n", total);
}

void cscan(int req[], int n, int head, int size) {
    int total = 0;
    for(int i = head; i < size; ++i) {
        for(int j = 0; j < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        }
    }

    head = 0;
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        } 
    }

    printf("C-SCAN total head movement: %d\n", total);
}

int main() {
    int n, head, size;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    int req[n];

    printf("Enter request queue:\n");
    for(int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size (e.g., 200): ");
    scanf("%d", &size);

    fcfs(req, n, head);
    sstf(req, n, head);
    scan(req, n, head, size);
    cscan(req, n, head, size);

    return 0;
}