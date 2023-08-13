#include <sys/types.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t stoveMutex[4];
int stoveFuel[4] = {100, 100, 100, 100};

void *routine(void *arg) {
    for (int i = 0; i < sizeof(stoveMutex) / sizeof(pthread_mutex_t); i++) {
        if (pthread_mutex_trylock(stoveMutex + i) == 0 ) {
            int fuelNeeded = rand() % 30;
            if (stoveFuel[i] < fuelNeeded) {
                printf("No more fuel\n");
            } else {
                stoveFuel[i] -= fuelNeeded;
                printf("Stove %d fuel left: %d\n", i, stoveFuel[i]);
            }
            usleep(500000);
            pthread_mutex_unlock(stoveMutex + i);
            break;
        } else {
            if (i == 3) {
                printf("No stove available yet waiting...\n");
                usleep(300000);
                i = -1;
            }
        }
    }
    return NULL;
}

int main(void) {
    srand(time(NULL));
    pthread_t th[10];
    for (int i = 0; i < sizeof(stoveMutex) / sizeof(pthread_mutex_t); i++)
        pthread_mutex_init(stoveMutex + i, NULL);

    for (int i = 0; i < 10; i++)
        pthread_create(th + i, NULL, routine, NULL);

    for (int i = 0; i < 10; i++)
        pthread_join(th[i], NULL);

    for (int i = 0; i < sizeof(stoveMutex) / sizeof(pthread_mutex_t); i++)
        pthread_mutex_destroy(stoveMutex + i);

    return 0;
}