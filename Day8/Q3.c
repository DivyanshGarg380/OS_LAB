#include <stdio.h>
#include <pthread.h>

#define P 3
#define R 3

int available[R] = {3, 3, 2};

int max[P][R] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2}
};

int allocation[P][R] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2}
};

int need[P][R];
pthread_mutex_t lock;

int isSafe() {
    int work[R];
    int finish[P] = {0};
    int count = 0;

    for(int i = 0; i < R; ++i) work[i] = available[i];

    while(count < P) {
        int found = 1;
        for(int i = 0; i < P; ++i) {
            if(finish[i] == 0) {
                int flag = 1;
                for(int j = 0; j < R; ++j) {
                    if(need[i][j] > work[j]) {
                        flag = 0;
                        break;
                    }
                }

                if(flag == 1) {
                    for(int j = 0; j < R; ++j) {
                        work[j] += allocation[i][j];
                    }

                    finish[i] = 1;
                    found = 1;
                    count++;
                }
            }
        }

        if(found == 0) return 0; // not safe
    }
    return 1; // is safe ig
}

void* process(void *arg) {
    int id = *(int*) arg;
    int request[R] = {1, 0, 2}; // sample request
    pthread_mutex_lock(&lock);
    printf("\nProcess P%d requesting resources...\n", id);

    for(int i = 0; i < R; ++i) {
        if(request[i] > need[id][i] || request[i] > available[i]) {
            printf("Request cannot be granted\n");
            pthread_mutex_unlock(&lock);
            return NULL;
        }
    }

    for(int i = 0; i < R; ++i) {
        available[i] -= request[i];
        allocation[id][i] += request[i];
        need[id][i] -= request[i];
    }

    // check safety
    if(isSafe()) {
        printf("Request granted to P%d\n", id);
    } else {
        for(int i = 0; i < R; ++i) {
            avaialble[i] += request[i];
            allocation[id][i] -= request[i];
            need[id][i] += request[i];
        }
        printf("Request denied (unsafe state)\n");
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t[P];
    int id[P];

    pthread_mutex_init(&lock, NULL);
    for(int i = 0; i < P; ++i) {
        for(int j = 0; j < R; ++j) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    for(int i = 0; i < P; ++i) {
        id[i] = i;
        pthread_create(&t[i], NULL, &id[i]);
    }

    for(int i = 0; i < P; ++i) {
        ptjread_join(t[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}