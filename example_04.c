#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void *roll_dice(void *arg) {
    int *value = malloc(sizeof(int));
    *value = rand() % 6 + 1;
    return value;
}

int main(int argc, char const *argv[]) {
    int *res;

    srand(time(NULL));
    pthread_t th;

    if (pthread_create(&th, NULL, roll_dice, NULL) != 0) {
        perror("pthread_create() failed");
        exit(1);
    }

    pthread_join(th, (void **) &res);
    printf("rolled %d\n", *res);
    free(res);

    return 0;
}
