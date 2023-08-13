#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define THREAD_NUM 4

sem_t sem;

void *routine(void *arg) {
    int index = *(int *) arg;
    int semVal;
    sem_wait(&sem);
    sleep(index + 1);
    sem_getvalue(&sem, &semVal);
    printf("(%d) current sem val after wait: %d\n", index, semVal);
    sem_post(&sem);
    sem_getvalue(&sem, &semVal);
    printf("(%d) current sem val after post: %d\n", index, semVal);
    free(arg);
    pthread_exit(0);
}

int main(void) {
    pthread_t th[THREAD_NUM];
    sem_init(&sem, 0, 4);

    for (int i = 0; i < THREAD_NUM; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(th + i, NULL, routine, arg);    
    }

    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(th[i], NULL);
 
    sem_destroy(&sem);
    return 0;
}