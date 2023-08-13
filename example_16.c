#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semaphore;

void *routine(void *arg) {
    sem_wait(&semaphore);
    sleep(1);
    printf("hello from thread %d\n", *(int*)arg);
    sem_post(&semaphore);
    pthread_exit(0);
}

int main(void) {
    pthread_t th[THREAD_NUM];

    sem_init(&semaphore, 0, 2);

    for (int i = 0; i < THREAD_NUM; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(th + i, NULL, routine, arg);
    }
    
    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&semaphore);

    return 0;
}