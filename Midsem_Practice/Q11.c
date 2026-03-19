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
        - Reads number of readers (R) and writers (W)
        - Reads number of operations each will perform
        - Stores a shared integer in shared memory
        - Displays input in tabular format
    
    - The parent creates multiple child processes using fork():
        - Some execute ./readerExec
        - Some execute ./writerExec using exec()

    - The programs readerExec and writerExec:
        - Use shared memory to access/update data
        - Use semaphores to implement Reader-Writer synchronization

    - Constraints:
        - Multiple readers can read simultaneously
        - Writers must have exclusive access
        - No reader should read while a writer is writing
    
    - Output:
        - Show read/write operations clearly with process IDs
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_KEY 1234

typedef struct {
    int val;
    int readCnt;
} shared_data;

int main() {
    int R, W;
    printf("Enter number of Readers: ");
    scanf("%d", &R);

    printf("Enter number of writers: ");
    scanf("%d", &W);

    int shmid = shmget(SHM_KEY, sizeof(shared_data), 0666 | IPC_CREAT);
    shared_data *data = (shared_data *)shmat(shmid, NULL, 0);

    data->val = 0;
    data->readCnt = 0;

    sem_t *mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    sem_t *wrt   = sem_open("/wrt", O_CREAT, 0666, 1);

    for(int i = 0; i < R; ++i){
        if(fork() == 0) {
            execl("./readerExec", "readerExec", NULL);
            exit(0);
        }
    }

    for(int i = 0; i < W; ++i) {
        if(fork() == 0) {
            execl("./writerExec", "writerExec", NULL);
            exit(0);
        }
    }

    for(int i = 0; i < R + W; ++i) wait(NULL);
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    sem_unlink("/mutex");
    sem_unlink("/wrt");

    return 0;
}

// readerExec.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_KEY 1234

typedef struct {
    int val;
    int readCnt;
} shared_data;

int main() {
    int shmid = shmget(SHM_KEY, sizeof(shared_data), 0666);
    shared_data *data = (shared_data *)shmat(shmid, NULL, 0);

    sem_t *mutex = sem_open("/mutex", 0);
    sem_t *wrt = sem_open("/wrt", 0);

    sem_wait(mutex);
    data->readCnt++;

    if(data->readCnt == 1) sem_wait(wrt);
    sem_post(mutex);
    printf("Reader %d reads value: %d\n", getpid(), data->val);
    sem_wait(mutex);
    data->readCnt--;
    if(data->readCnt == 0) sem_post(wrt);
    sem_post(mutex);
    return 0;
}

// writerExec.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_KEY 1234

typedef struct {
    int val;
    int readCnt;
} shared_data;

int main() {
    int shmid = shmget(SHM_KEY, sizeof(shared_data), 0666);
    shared_data *data = (shared_data *)shmat(shmid, NULL, 0);

    sem_t *wrt = sem_open("/wrt", 0);
    sem_wait(wrt);
    data->val++;
    printf("Writer %d writes value: %d\n", getpid(), data->val);
    sem_post(wrt);
    return 0;
}
