#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void *print_prime(void *arg) {
    printf("%d\n", *(int*)arg);
    return NULL;
}

int main(int argc, char const *argv[]) {
    pthread_t th[10];

    for (int i = 0; i < 10; i++)
        pthread_create(th + i, NULL, print_prime, primes+i);

    for (int i = 0; i < 10; i++)
        pthread_join(th[i], NULL);

    return 0;
}
