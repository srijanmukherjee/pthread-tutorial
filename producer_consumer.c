#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define THREAD_NUM  8

int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutexBuffer = PTHREAD_MUTEX_INITIALIZER;
sem_t semEmpty, semFull;

void *consumer(void *arg) {
    while (1) {
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        // remove from the buffer
        int y = buffer[--count];
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        // consume
        printf("consumed %d\n", y); 
    }

    pthread_exit(0);
}

void *producer(void *arg) {
    while (1) {
        // produce
        int x = rand() % 100; 
        sleep(1);
        
        // add to buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count++] = x;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }

    pthread_exit(0);
}

int main(void) {
    srand(time(NULL));

    pthread_t th[THREAD_NUM];

    sem_init(&semEmpty, 0, BUFFER_SIZE);
    sem_init(&semFull, 0, 0);

    for (int i = 0; i < THREAD_NUM; i++) {
        if (i > 0) {
            pthread_create(th + i, NULL, producer, NULL);
        } else {
            pthread_create(th + i, NULL, consumer, NULL);
        }
    }

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}
