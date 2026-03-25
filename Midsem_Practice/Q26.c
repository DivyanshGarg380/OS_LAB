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
    Design a C program that performs the following:

    The parent process creates 2 child processes using fork().
    - Child 1:
        - Generates N integers (taken from user).
        - Writes them to a pipe.
    
    - Child 2:
        - Reads integers from the pipe.
        - Uses 2 threads:
            - Thread 1 → computes sum of even numbers
            - Thread 2 → computes sum of odd numbers
        - Uses a mutex to safely update shared sums.
    
    - After computation:
        - Child 2 writes results (even_sum, odd_sum) into shared memory.
    
    - The parent:
        - Waits for both children.
        - Reads results from shared memory.
        - Prints:
            - Even Sum = X
            - Odd Sum = Y
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <pthread.h>

#define SIZE 100;

int arr[SIZE], n;
int even = 0, odd = 0;

pthread_mutex_t lock;

void *sum_even(void *arg){
    for(int i = 0; i < n; ++i){
        if(arr[i] % 2 == 0) {
            pthread_mutex_lock(&lock);
            even += arr[i];
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

void *sum_odd(void *arg){
    for(int i = 0; i < n; ++i){
        if(arr[i] % 2 != 0) {
            pthread_mutex_lock(&lock);
            odd += arr[i];
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main() {
    int fd[2];
    pipe(fd);

    int shmid = shmget((key_t)1234, 2 * sizeof(int), 0666 | IPC_CREAT);
    int *memo = (int *)shmat(shmid, NULL, 0);

    pid_t pid1 = fork();
    if(pid1 == 0) {
        close(fd[0]);
        printf("Enter number of elements: ");
        scanf("%d", &n);

        printf("Enter elements:\n");
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }

        write(fd[1], &n, sizeof(int));
        write(fd[1], arr, n * sizeof(int));

        close(fd[1]);
        exit(0);
    }

    pid_t pid2 = fork();
    if(pid2 == 0) {
        close(fd[1]);
        read(fd[0], &n, sizeof(int));
        read(fd[0], arr, sizeof(int) * n);

        close(fd[0]);

        pthread_t t1, t2;
        pthread_mutex_init(&lock, NULL);

        pthread_create(&t1, NULL, sum_even, NULL);
        pthread_create(&t2, NULL, sum_odd, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        memo[0] = even;
        memo[1] = odd;

        pthread_mutex_destroy(&lock);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);

    printf("Even Sum = %d\n", memo[0]);
    printf("Odd Sum = %d\n", memo[1]);

    shmdt(memo);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

