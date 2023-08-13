#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void *routine(void *arg) {
    int start = *(int *) arg;
    int sum = 0;
    for (int i = start; i < start + 5; i++)
        sum += primes[i];
    *(int*)arg = sum;
    return arg;
}

int main(int argc, char const *argv[]) {
    pthread_t th[2];
    int *args[2];
    int *ret;
    int res = 0;

    for (int i = 0; i < 2; i++) {
        args[i] = malloc(sizeof(int));
        *args[i] = i * 5;
        pthread_create(th + i, NULL, routine, args[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(th[i], (void **) &ret);
        res += *ret;
        free(ret);
    }

    printf("sum = %d\n", res);

    return 0;
}
