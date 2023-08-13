#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 8

void *routine(void *arg) {
    sleep(1);
    printf("finished execution, pid: %d\n", getpid());
    return NULL;
}

int main(void) {
    pthread_t th[THREAD_NUM];

    printf("main pid: %d\n", getpid());

    pthread_attr_t detachedThread;
    pthread_attr_init(&detachedThread);
    pthread_attr_setdetachstate(&detachedThread, PTHREAD_CREATE_DETACHED);

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_create(th + i, &detachedThread, routine, NULL);

    // for (int i = 0; i < THREAD_NUM; i++)
    //     if (pthread_join(th[i], NULL) != 0)
    //         perror("pthread_join() failed");

    pthread_attr_destroy(&detachedThread);
    pthread_exit(0);
}