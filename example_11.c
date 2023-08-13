#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_barrier_t barrier;

void *routine(void *arg) {
    while (1) {
        printf("Waiting at the barrier...\n");
        sleep(1);
        pthread_barrier_wait(&barrier);
        printf("passsed the barrier\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    pthread_t th[10];
    size_t n = sizeof(th) / sizeof(pthread_t);

    pthread_barrier_init(&barrier, NULL, 7);

    for (int i = 0; i < n; i++)
        pthread_create(th + i, NULL, routine, NULL);

    for (int i = 0; i < n; i++)
        pthread_join(th[i], NULL);

    pthread_barrier_destroy(&barrier);

    return 0;
}