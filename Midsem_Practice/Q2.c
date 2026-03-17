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
    4 threads
    Shared array
    Mutex
    Stop < 40000000
*/

#include <stdio.h>
#include <pthread.h>

int arr[2] = {0,1};
pthread_mutex_t lock;

void* func(void* arg){
    while(1) {
        pthread_mutex_lock(&lock);
        int next = arr[0] + arr[1];
        if(next >= 40000000) {
            pthread_mutex_unlock(&lock);
            break;
        }
        printf("Thread %lu: %d\n", pthread_self(), next);
        arr[0] = arr[1];
        arr[1] = next;
        pthread_mutex_unlock(&lock);
    }
    return null;
}

int main() {
    pthread_t t[4];
    pthread_mutex_init(&lock, NULL);
    for(int i = 0; i < 4; ++i) pthread_create(&t[i], NULL, func, NULL);
    for(int i = 0; i < 4; ++i) pthread_join(&t[i], NULL);
    return 0;
}