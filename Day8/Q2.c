#include <stdio.h>

int main() {
    int n = 5, m = 3;

    int allocation[5][3] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 3},
        {2, 1, 1},
        {0, 0, 2}
    };

    int request[5][3] = {
        {0, 0, 0},
        {2, 0, 2},
        {0, 0, 0},
        {1, 0, 0},
        {0, 0, 2}
    };

    int available[3] = {0, 0, 0};
    int finish[5] = {0};
    int work[3];
    int i, j, k;

    // work = available
    for(int i = 0; i < n; ++i) {
        work[i] = available[i];
    }

    for(int i = 0; i < n; ++i) {
        int flag = 0;
        for(int j = 0; j < m; ++j) {
            if(allocation[i][j] != 0) {
                flag = 1;
                break;
            }
        }
        if(flag == 0) finish[i] = 1;
    }

    // detection algo
    for(i = 0; i < n; ++i) {
        if(finish[i] == 0) {
            int flag = 1;
            for(j = 0; j < m; ++j) {
                if(request[i][j] > work[j]) {
                    flag = 0;
                    break;
                }
            }

            if(flag == 1) {
                for(k = 0; k < m; ++k) {
                    work[k] += allocation[i][k];
                }

                finish[i] = 1;
                i = -1; // restart loop
            }
        }
    }

    // now check for any deadlock
    int deadlock = 0;
    for(i = 0; i < n; ++i) {
        if(finish[i] == 0) {
            deadlock = 1;
            printf("Prcess P%d is deadlocked\n", i);
        }
    }

    if(!deadlock) printf("No deadlock\n");
    return 0;
}