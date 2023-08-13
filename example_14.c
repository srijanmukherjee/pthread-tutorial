#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 8

pthread_mutex_t mutexFuel = PTHREAD_MUTEX_INITIALIZER;
int fuel = 0;

pthread_mutex_t mutexWater = PTHREAD_MUTEX_INITIALIZER;
int water = 0;

void *routine(void *arg) {
    if (rand() % 2 == 0) {
        pthread_mutex_lock(&mutexFuel);
        sleep(1);
        pthread_mutex_lock(&mutexWater);
    } else { 
        pthread_mutex_lock(&mutexWater);
        sleep(1);
        pthread_mutex_lock(&mutexFuel);
    }

    fuel += 50;
    water = fuel;
    printf("incremented fuel to %d\n", fuel);
    
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexWater);
    pthread_exit(0);
}

int main(void) {
    pthread_t th[THREAD_NUM];

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_create(th + i, NULL, routine, NULL);
    
    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);

    printf("Fuel: %d\n", fuel);
    printf("Water: %d\n", water);

    return 0;
}