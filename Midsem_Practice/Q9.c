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
        - Reads number of cars and their IDs
        - Stores them in shared memory
        - The parent creates a child process using fork()
    
    - The child executes an external program ./parkingExec using exec()
    - The parkingExec program:
        - Uses shared memory to access car IDs
        - Creates threads (one per car)
    - Uses a semaphore to manage parking slots (maximum 4 slots)
    - Behavior:
        - If slots available → allocate parking
        - If full → car waits
        - After some time → car leaves and releases slot
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {
    int n;
    printf("Enter number of cars: ");
    scanf("%d", &n);

    int shmid = shmget(IPC_PRIVATE, (n + 1) * sizeof(int), 0666 | IPC_CREAT);
    int *data = (int *)shmat(shmid, NULL, 0);

    data[0] = n;
    printf("Enter car Id's: "\n);
    for(int i = 1; i <= n; ++i) {
        scanf("%d", &data[i]);
    }

    printf("\nCar List:\n");
    for(int i = 1; i <= n; ++i) printf("Car %d\n", data[i]);
    
    pid_t pid = fork();
    if(pid == 0) {
        execl("./parkingExec", "parkingExec", NULL);
        perror("exec failed");
        exit(1);
    }

    wait(NULL);
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

// parkingExec.c file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

int n;
sem_t *slots;

void* park(void *arg){
    int id = *(int*)arg;

    printf("Car %d requesting parking\n", id);
    sem_wait(slots);

    printf("Car %d allocated slot\n", id);
    sleep(2);

    printf("Car %d leaving parking\n", id);
    sem_post(slots);

    return NULL;
}

int main() {
    int shmid = shmget(IPC_PRIVATE, 0, 0666);
    int *cars = (int *)shmat(shmid, NULL, 0);

    n = cars[0];

    slots = sm_open("/parking_sem", O_CREAT, 0666, 4);
    pthread_t threads[n];
    for(int i = 1; i <= n; ++i){
        pthread_create(&threads[i-1], NULL, park, &car[i]);
    }
    for(int i = 0; i < n; ++i) {
        pthread_join(threads[i], NULL);
    }
    sem_unlink("/parking_sem");
    return 0;
}