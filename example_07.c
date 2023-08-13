#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *arg) {
    // pthread_mutex_lock(&mutex);
    if (pthread_mutex_trylock(&mutex) == 0) {
        printf("got lock\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
    } else {
        printf("Didn't get the lock\n");
    }

    return NULL;
}

int main(int argc, char const *argv[]) {
    pthread_t th[4];

    for (int i = 0; i < 4; i++)
        pthread_create(th + i, NULL, routine, NULL);

    for (int i = 0; i < 4; i++)
        pthread_join(th[i], NULL);

    return 0;
}
