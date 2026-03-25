#include <stdio.h>

int main() {
    int n = 5, m = 3;
    int allocation[5][3] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int max[5][3] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int available[3] = {3, 3, 2};
    int need[5][3];
    int finish[5] = {0};
    int safeSeq[5];
    int work[3];
    int i, j, k, count = 0;

    // need = max - allocation
    for(i = 0; i < n; ++i) {
        for(j = 0; j < m; ++j) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // work = available
    for(int i = 0; i < m; ++i) work[i] = available[i];

    while(count < n) {
        for(i = 0; i < n; ++i) {
            if(finish[i] == 0) {
                int flag = 1;

                // check need <= work
                for(j = 0; j < m; ++j) {
                    if(need[i][j] > work[j]) {
                        flag = 0;
                        break;
                    }
                }

                // if possible -> execute process
                if(flag == 1) {
                    for(k = 0; k < m; ++k) work[k] += allocation[i][k];

                    safeSeq[count++] = i;
                    finish[i] = 1;
                }

            }
        }
    }

    // check safe state
    if(count == n) {
        printf("System is SAFE\nSafe Sequence: ");
        for(int i = 0; i < n; ++i) printf("P%d ", safeSeq[i]);
    } else {
        printf("System is NOT SAFE\n");
    }
    return 0;
}