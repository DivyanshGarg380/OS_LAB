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
    Develop a C program where:
    
    - The parent process:
        - Reads n integers from the user
        - Sends the data to a child process using a pipe

    - The parent creates a child process using fork()
    - The child executes an external program ./pipeWorker using exec()
    - The pipeWorker program:
        - Reads data from the pipe
        - Stores it in shared memory

    - Creates 3 threads:
        - Thread 1 → sort the array
        - Thread 2 → calculate sum
        - Thread 3 → find maximum

    - Use a mutex to synchronize access to shared memory
    - Each thread prints its result with thread ID
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter elements:\n");
    for(int i = 0; i < n; ++i) scanf("%d", &arr[i]);

    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if(pid == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execl("./pipeWorker", "pipeWorker", NULL);
        perror("exec failed");
        exit(1);
    } else {
        close(fd[0]);
        write(fd[1], &n, sizeof(int));
        write(fd[1], arr, sizeof(int)*n);
        close(fd[1]);
        wait(NULL);
    }
}

// pipeWorker.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>


#define SHM_KEY 5678

int *data;
int n;
pthread_mutex_t lock;

void *sort(void* arg) {
    pthread_mutex_lock(&lock);

    for(int i = 0; i < n - 1; ++i) {
        for(int j = i + 1; j < n; ++j) {
            if(data[i] > data[j]) {
                int temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }

    printf("Thread %lu -> Sorted: ", pthread_self());
    for (int i = 0; i < n; i++) printf("%d ", data[i]);
    printf("\n");

    pthread_mutex_unlock(&lock);
    return NULL;
}

void *calc_sum(void *arg) {
    pthread_mutex_lock(&lock);

    int sum = 0;
    for(int i = 0; i < n; ++i) sum += data[i];

     printf("Thread %lu -> Sum = %d\n", pthread_self(), sum);

    pthread_mutex_unlock(&lock);
    return NULL;
}

void* find_max(void *arg) {
    pthread_mutex_lock(&lock);

    int max = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] > max) max = data[i];
    }

    printf("Thread %lu -> Max = %d\n", pthread_self(), max);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    read(STDIN_FILENO, &n, sizeof(int));
    int shmid = shmget(SHM_KEY, n * sizeof(int), 0666 | IPC_CREAT);
    data = (int *)shmat(shmid, NULL, 0);

    read(STDIN_FILENO, data, sizeof(int) * n);
    pthread_mutex_init(&lock, NULL);
    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, sort, NULL);
    pthread_create(&t2, NULL, calc_sum, NULL);
    pthread_create(&t3, NULL, find_max, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock);

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}