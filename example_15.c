#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 8

pthread_mutex_t mutex;

void *routine(void *arg) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex);
    printf("executing critical section\n");
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

int main(void) {
    pthread_t th[THREAD_NUM];

    pthread_mutexattr_t recursiveMutex;
    pthread_mutexattr_init(&recursiveMutex);
    pthread_mutexattr_settype(&recursiveMutex, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&mutex, &recursiveMutex);

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_create(th + i, NULL, routine, NULL);
    
    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&recursiveMutex);

    return 0;
}