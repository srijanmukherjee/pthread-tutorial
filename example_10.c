#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void *routine(void *arg) {
    int *result = malloc(sizeof(int));
    *result = rand() % 6 + 1;
    sleep(2);
    printf("result: %d\n", *result);
    pthread_exit((void *) result);
}

int main(void) {
    srand(time(NULL));
    pthread_t th;
    int *res;

    pthread_create(&th, NULL, routine, NULL);
    // pthread_exit() inside the main thread does not kill the process
    // waits for the threads to finish before exiting
    pthread_exit(0);
    pthread_join(th, (void **)&res);

    printf("Result: %d\n", *res);
    free(res);
    return 0;
}