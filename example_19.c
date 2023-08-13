// binary semaphore example 2
// see example_18.c

#include <sys/types.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 1

sem_t semFuel;
int *fuel;

void *routine(void *arg) {
    *fuel += 50;
    printf("current value is %d\n", *fuel);
    sem_post(&semFuel);
    pthread_exit(0);
}

int main(void) {
    pthread_t th[THREAD_NUM];
    sem_init(&semFuel, 0, 0);

    fuel = malloc(sizeof(int));
    *fuel = 0;

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(th + i, NULL, routine, NULL);
    }

    sem_wait(&semFuel);
    printf("deallocating fuel\n");
    free(fuel);

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&semFuel);
    return 0;
}