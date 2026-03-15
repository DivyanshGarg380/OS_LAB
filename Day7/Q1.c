#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5

int buffer[SIZE];
int f = -1, r = -1;

int produced = 0;
int consumed = 0;

sem_t mutex, full, empty;

void *produce(void *arg) {
	for(int i = 0; i < 20; ++i){
		while(produced - consumed >= 10);
		sem_wait(&empty);
		sem_Wait(&mutex);

		r = (r + 1) % SIZE;
		buffer[r] = i;
		produced++;

		printf("Produced: %d\n", i);
		
		sem_post(&mutex);
		sem_post(&full);

		sleep(1);
	}
	return NULL;
}


void *consume(void *arg) {
	for(int i = 0; i < 20; ++i) {
		sem_wait(&full);
		sem_wait(&mutex);
	
		f = (f + 1) % SIZE;
		int item = buffer[f];
		consumed++;

		printf("Consumed: %d\n", item);

		sem_post(&mutex);
		sem_post(&empty);

		sleep(2);

	}
	return NULL;
}

int main() {
	pthread_t t1, t2;
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, SIZE);

	pthread_create(&t1, NULL, produce, NULL);
	pthread_create(&t2, NULL, consume, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);

	return 0;
}
