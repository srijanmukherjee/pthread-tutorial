#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/syscall.h>

#define THREAD_NUM 8

pthread_barrier_t barrier;
pthread_barrier_t statusBarrier;

int diceValues[THREAD_NUM];
int status[THREAD_NUM];

void *rollDice(void *arg) {
    printf("tid: %d\n", (pid_t) syscall(SYS_gettid));
    int index = *(int *) arg;
    diceValues[index] = rand() % 6 + 1;
    
    pthread_barrier_wait(&barrier);
    pthread_barrier_wait(&statusBarrier);
    
    if (status[index] == 1)
        printf("(%d rolled %d) I won\n", index, diceValues[index]);
    else
        printf("(%d rolled %d) I lost\n", index, diceValues[index]);

    free(arg);
    pthread_exit(NULL);
}

int main(void) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];

    // THREAD_NUM + 1: extra one is for the main thread
    pthread_barrier_init(&barrier, NULL, THREAD_NUM + 1);
    pthread_barrier_init(&statusBarrier, NULL, THREAD_NUM + 1);

    for (int i = 0; i < THREAD_NUM; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(th + i, NULL, rollDice, arg);
    }

    pthread_barrier_wait(&barrier);

    int max = 0;
    for (int i = 0; i < THREAD_NUM; i++) 
        if (diceValues[i] > max)
            max = diceValues[i];
    
    for (int i = 0; i < THREAD_NUM; i++)
        status[i] = (diceValues[i] == max) ? 1 : 0;

    pthread_barrier_wait(&statusBarrier);

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);

    pthread_barrier_destroy(&barrier);
    pthread_barrier_destroy(&statusBarrier);

    return 0;
}