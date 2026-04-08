#include <stdio.h>

void firstFit(int holes[], int h, int process[], int p) {
    int temp[h];
    for(int i = 0; i < h; ++i) temp[i] = holes[i];

    for(int i = 0; i < p; ++i) {
        int allocated = 0;
        for(int j = 0; j < h; ++j) {
            if(temp[j] >= process[i]) {
                printf("Process %d allocated to Hole %d\n", i + 1, j + 1);
                temp[j] -= process[i];
                allocated = 1;
                break;
            }
        }
        if(!allocated) {
            printf("Process %d not allocated\n", i + 1);
        }
    }
}

void bestFit(int holes[], int h, int process[], int p) {
    int temp[h];
    for(int i = 0; i < h; ++i) temp[i] = holes[i];

    for(int i = 0; i < p; ++i) {
        int best = -1;
        for(int j = 0; j < h; ++j) {
            if(temp[j] >= process[i]) {
                if(best == -1 || temp[j] < temp[best]) {
                    best = j;
                }
            }
        }

        if(best != -1) {
            printf("Process %d allocated to Hole %d\n", i+1, best+1);
            temp[best] -= process[i];
        } else {
            printf("Process %d not allocated\n", i + 1);
        }
    }
}

void worstFit(int holes[], int h, int process[], int p) {
    int temp[h];
    for(int i = 0; i < h; ++i) temp[i] = holes[i];

    for(int i = 0; i < p; ++i) {
        int worst = -1;
        for(int j = 0; j < h; ++j) {
            if(temp[j] >= process[i]) {
                if(worst == -1 || temp[j] > temp[worst]) worst = j;
            }
        }

        if(worst != -1) {
            printf("Process %d allocated to Hole %d\n", i + 1, worst + 1);
            temp[worst] -= process[i];
        } else {
            printf("Process %d not allocated\n", i + 1);
        }
    }
}

int main() {
    int holes[5] = {100, 500, 200, 300, 600};
    int process[4] = {212, 417, 112, 426};

    firstFit(holes, 5, process, 4);
    bestFit(holes, 5, process, 4);
    worstFit(holes, 5, process, 4);

    printf("\nBest Fit uses memory most efficiently.\n");

    return 0;
}