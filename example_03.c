#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_COUNT 8

int mails = 0;
pthread_mutex_t mutex /*= PTHREAD_MUTEX_INITIALIZER*/;

void *routine(void *arg) {
    for (int i = 0; i < 100000; i++) { 
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t t[THREAD_COUNT];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        if (pthread_create(t + i, NULL, routine, NULL) < 0) {
            perror("failed to create some threads");
            goto cleanup;
        }

cleanup:
    for (int i = 0; i < THREAD_COUNT; i++) 
        pthread_join(t[i], NULL);

    pthread_mutex_destroy(&mutex);
    printf("number of mails: %d\n", mails);

    return 0;
}
