#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *routine(void *arg) {
    printf("thread id: %ld\n", pthread_self());
    sleep(3);
    printf("ending thread\n");
}

int main(int argc, char const *argv[])
{
    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, routine, NULL) < 0) {
        perror("failed to create thread");
        exit(1);
    }

    if (pthread_create(&t2, NULL, routine, NULL) < 0) {
        perror("failed to create thread");
        exit(1);
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
