#include <sys/types.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semFuel;
int fuel = 50;

void *routine(void *arg) {
    sem_wait(&semFuel);
    fuel += 50;
    printf("current value is %d\n", fuel);
    sem_post(&semFuel);
    pthread_exit(0);
}

int main(void) {
    pthread_t th[THREAD_NUM];
    sem_init(&semFuel, 0, 1);

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(th + i, NULL, routine, NULL);
    }

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&semFuel);
    return 0;
}