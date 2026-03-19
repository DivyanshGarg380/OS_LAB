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
        - Stores them in shared memory
        - Displays the entered values

    - The parent creates a child process using fork()
    - The child executes an external program ./statsExec using exec()
    - The statsExec program:
        - Accesses the shared memory
        - Creates 3 threads using pthreads

    - Threads perform:
        - Thread 1 → calculate sum
        - Thread 2 → calculate average
        - Thread 3 → find maximum
    
    - Use a mutex to ensure synchronized access to shared memory
    - Each thread prints its result along with thread ID
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int shmid = shmget(IPC_PRIVATE, (n+1)*sizeof(int), O066 | IPC_CREAT);
    int *data = (int *)shmat(shmid, NULL, 0);

    data[0] = n;

    printf("Enter elements:\n");
    for(int i = 1; i <= n; ++i) scanf("%d", &data[i]);

    printf("\nInput Data:\n");
    for(int i = 1; i <= n; ++i) printf("%d", data[i]);

    pid_t pid = fork();
    if(pid == 0) {
        execl("./statsExec", "statsExec", NULL);
        perror("exec failed");
        exit(1);
    }

    wait(NULL);
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

// statsExec.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <unistd.h>

int n;
pthread_mutex_t lock;

void* calc_sum(void *arg) {
    pthread_mutex_lock(&lock);
    int sum = 0;
    for(int i = 1; i <= n; ++i) {
        sum += data[i];
    }
    printf("Thread %lu -> Sum = %d\n", pthread_self(), sum);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *calc_avg(void *arg){
    pthread_mutex_lock(&lock);
    int sum = 0;
     for (int i = 1; i <= n; i++) {
        sum += data[i];
    }
    float avg = (float)sum / n;

    printf("Thread %lu -> Average = %.2f\n", pthread_self(), avg);

    pthread_mutex_unlock(&lock);
    return NULL;
}

void *calc_max(void *arg){
    pthread_mutex_lock(&lock);

    int max = data[1];
    for (int i = 2; i <= n; i++) {
        if (data[i] > max) max = data[i];
    }

    printf("Thread %lu -> Max = %d\n", pthread_self(), max);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    int shmid = shmget(IPC_PRIVATE, 0, 0666);
    int *data = (int *)shmat(shmid, NULL, 0);

    n = data[0];
    pthread_mutex_init(&lock, NULL);
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, calc_sum, NULL);
    pthread_create(&t2, NULL, calc_avg, NULL);
    pthread_create(&t3, NULL, calc_max, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}