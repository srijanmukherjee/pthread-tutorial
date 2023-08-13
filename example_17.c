#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 16

sem_t loginSemaphore;

void *routine(void *args) {
    printf("(%d) waiting in the login queue\n", *(int *) args);
    sem_wait(&loginSemaphore);

    // critical section
    printf("(%d) logged in\n", *(int*)args);
    sleep(rand() % 5 + 1);
    printf("(%d) logged out\n", *(int*)args);

    sem_post(&loginSemaphore);
    free(args);
    pthread_exit(0);
}

int main(void) {
    pthread_t th[THREAD_NUM];

    sem_init(&loginSemaphore, 0, 12);

    for (int i = 0; i < THREAD_NUM; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(th + i, NULL, routine, arg);
    }

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&loginSemaphore);

    return 0;
}