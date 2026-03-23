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
    Implement Reader Writer simulation giving more priority to the writer 
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int readCnt = 0;
sem_t mutex, wrt, queue;

void *reader(void *arg) {
    sem_wait(&queue);
    sem_wait(&mutex);

    readCnt++;
    if(readCnt == 1) sem_wait(&wrt);

    sem_post(&mutex);
    sem_post(&queue);

    printf("Reader reading\n");

    sem_wait(&mutex);
    readCnt--;
    if(readCnt == 0) sem_post(&wrt);
    sem_post(&mutex);
    return NULL;
}

void* writer(void* arg) {
    sem_wait(&queue);
    sem_wait(&wrt);
    sem_post(&queue);

    printf("Writer writing\n");
    sem_post(&wrt);
    return NULL;
}

int main() {
    pthread_t r[3], w[2];

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    sem_init(&queue, 0, 1);

    for (int i = 0; i < 3; i++) pthread_create(&r[i], NULL, reader, NULL);

    for (int i = 0; i < 2; i++) pthread_create(&w[i], NULL, writer, NULL);

    for (int i = 0; i < 3; i++) pthread_join(r[i], NULL);

    for (int i = 0; i < 2; i++) pthread_join(w[i], NULL);
}