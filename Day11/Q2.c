#include <stdio.h>
#include <stdlib.h>

void look(int req[], int n, int head) {
    int total = 0;
    for(int i = head; i >= 0; --i) {
        for(int j = 0; j  < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        }
    }

    for(int i = head; i < 200; ++i) {
        for(int j = 0; j < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        }
    }

    printf("LOOK total head movement: %d\n", total);
}

void clook(int req[], int n, int head) {
    int total = 0;
    for(int i = head; i < 200; ++i) {
        for(int j = 0; j < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        }
    }

    int mini = 1e4;
    for(int i = 0; i < n; ++i) {
        if(req[i] < mini) {
            mini = req[i];
        }
    }

    head = mini;
    for(int i = mini; i < 200; ++i) {
        for(int j = 0; j < n; ++j) {
            if(req[j] == i) {
                total += abs(head - i);
                head = i;
            }
        }
    }

    printf("C-LOOK total head movement: %d\n", total);
} 

int main() {
    int n, head;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    int req[n];

    printf("Enter request queue:\n");
    for(int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    look(req, n, head);
    clook(req, n, head);

    return 0;
}