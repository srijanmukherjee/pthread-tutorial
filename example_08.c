#include <sys/types.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int fuel = 0;
pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

void *fuel_filling(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 32;
        printf("Fueling fuel...%d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        // pthread_cond_singal(&condFuel);        // wakes a single waiting thread
        if (fuel >= 40)
            pthread_cond_broadcast(&condFuel);  // wakes up all the waiting threads
        
        sleep(1);
    }
    return NULL;
}

void *car(void *arg) {
    pthread_mutex_lock(&mutexFuel);
    while (fuel < 40) {
        printf("No fuel. Waiting...\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
        // Equivalent to
        // 1. pthread_mutex_unlock(&mutexFuel)
        // 2. wait for signal on condFuel
        // 3. pthread_mutex_lock(&mutexFuel)
    }

    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
    return NULL;
}

int main(void) {
    pthread_t th[10];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);

    for (int i = 0; i < 10; i++) {
        if (i >= 8)
            pthread_create(th + i, NULL, fuel_filling, NULL);
        else
            pthread_create(th + i, NULL, car, NULL);
    }

    for (int i = 0; i < 10; i++)
        pthread_join(th[i], NULL);

    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}