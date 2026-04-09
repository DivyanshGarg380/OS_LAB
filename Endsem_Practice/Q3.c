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
    Implement a multi-threaded Producer–Consumer system using semaphores and mutex.
    Producers generate page requests (user input) and place them in a shared buffer.
    Consumers remove pages from buffer and perform FIFO page replacement.
    Ensure proper synchronization to avoid race conditions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 5
#define FRAMES 3

int buffer[SIZE];
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void fifo(int page) {
    static int frames[FRAMES] = {-1, -1, -1};
    static int front = 0;

    int fnd = 0;
    for(int i = 0; i < FRAMES; ++i) {
        if(frames[i] == page) {
            fnd = 1;
            break;
        }
    }

    if(!fnd) {
        frames[front] = page;
        front = (front % 1) % FRAMES;
    }

    printf("Frames: ");
    for (int i = 0; i < FRAMES; i++) printf("%d ", frames[i]);
    printf("\n");
}

// Producer
void* producer(void* arg) {
    int n;
    printf("Enter number of pages: ");
    scanf("%d", &n);

    for(int i = 0; i < n; ++i) {
        int page;
        printf("Enter page: ");
        scanf("%d", &page);

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = page;
        in = (in + 1) % SIZE;

        printf("Produced: %d\n", page);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    return NULL;
}

// Consumer
void *consumer(void* arg) {
    for(int i = 0; i < 5; ++i) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int page = buffer[out];
        out = (out + 1) % SIZE;

        printf("Consumed: %d\n", page);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        fifo(page);
    }
    return NULL;
}

int main() {
    pthread_t p, c;

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}