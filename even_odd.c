#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <semaphore.h>

#define COUNT_MAX 10

int count = 1;
sem_t semOdd;
sem_t semEven;

void *even(void *args) {
    while (count <= COUNT_MAX) {
        sem_wait(&semEven);
        printf("(%d) %d is even\n", (pid_t) syscall(SYS_gettid), count);
        count++;
        sem_post(&semOdd);
    }

    pthread_exit(0);
}

void *odd(void *args) {
    while (count <= COUNT_MAX) {
        sem_wait(&semOdd);
        if (count <= COUNT_MAX)
            printf("(%d) %d is odd\n", (pid_t) syscall(SYS_gettid), count);
        count++;
        sem_post(&semEven);
    }
    pthread_exit(0);
}

int main(void) {
    pthread_t th[2];

    sem_init(&semEven, 0, 0);
    sem_init(&semOdd, 0, 1);

    for (int i = 0; i < 2; i++) {
        if (i % 2 == 0)
            pthread_create(th + i, NULL, even, NULL);
        else
            pthread_create(th + i, NULL, odd, NULL);
    }

    for (int i = 0; i < 2; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&semEven);
    sem_destroy(&semOdd);

    return 0;
}