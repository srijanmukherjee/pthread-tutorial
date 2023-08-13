// Race condition demonstration

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_COUNT 6

int mails = 0;

void *routine(void *arg) {
    for (int i = 0; i < 100000; i++) 
        mails++;
        // instructions:
        //     read mails
        //     increment
        //     write mails
}

int main(int argc, char const *argv[])
{
    pthread_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_create(&t[i], NULL, routine, NULL);

    for (int i = 0; i < THREAD_COUNT; i++) 
        pthread_join(t[i], NULL);

    printf("number of mails: %d\n", mails);

    return 0;
}
