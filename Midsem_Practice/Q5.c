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
    Printer Synchronization problem -> visit labxam.vercel.app to view complete question
*/

// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct Student {
    int id;
    int pages;
};

int main() {
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);

    int shmid = shmget(1234, sizeof(struct Student)*(n+1), 0666, IPC_CREAT);
    struct Student *s = (struct Student *)shmat(shmid, NULL, 0);

    s[0].id = n;
    for (int i = 1; i <= n; i++) {
        printf("Enter Student ID and Pages: ");
        scanf("%d %d", &s[i].id, &s[i].pages);
    }
    printf("\nStudentID\tPages\n");
    for (int i = 1; i <= n; i++) {
        printf("%d\t\t%d\n", s[i].id, s[i].pages);
    }

    printf("\nExecuting printerAccess program...\n\n");

    pid_t pid = fork();

    if (pid == 0) {
        execl("./printerAccess", "printerAccess", NULL);
        perror("exec failed");
        exit(1);
    }

    wait(NULL);

    shmdt(s);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

// printerAccess.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

#define PRINTERS 3

struct Student {
    int id;
    int pages;
};

sem_t printers;
pthread_mutex_t lock;

int printer_no = 1;

void* printJob(void *arg) {
    struct Student s = *(struct Student)* arg;
    printf("Student %d requesting printer\n", s.id);

    if (sem_trywait(&printers) != 0) {
        printf("All printers busy. Student %d waiting...\n", s.id);
        sem_wait(&printers);
    }

    pthread_mutex_lock(&lock);
    int assigned = printer_no;
    printer_no = (printer_no % PRINTERS) + 1;
    pthread_mutex_unlock(&lock);

    printf("Student %d allocated Printer %d\n", s.id, assigned);
    printf("Student %d printing %d pages\n\n", s.id, s.pages);

    sleep(1);

    printf("Student %d finished printing and released printer\n\n", s.id);

    sem_post(&printers);

    return NULL;
}

int main() {
    int shmid = shmget(1234, sizeof(struct Student) * 100, 0666);
    struct Student *s = (struct Student *)shmat(shmid, NULL, 0);

    int n = s[0].id;

    sem_init(&printers, 0, PRINTERS);
    pthread_mutex_init(&lock, NULL);

    pthread_t t[n];

    for (int i = 0; i < n; i++) {
        pthread_create(&t[i], NULL, printJob, &s[i + 1]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(t[i], NULL);
    }

    sem_destroy(&printers);
    pthread_mutex_destroy(&lock);

    shmdt(s);

    return 0;
}