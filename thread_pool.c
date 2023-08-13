#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define QUEUE_CAPACITY 512
#define WORKER_COUNT   8

typedef struct task {
    void* (*routine)(void *);
    void *args;
} task;

typedef struct queue {
    task *tasks;
    size_t back;
    size_t front;
    size_t capacity;
} queue;

void queue_init(queue *q);
void queue_destroy(queue *q);
int enqueue_task(queue *q, void* (*start_routine)(void *), void *args);
int dequeue_task(queue *q, task *task);

queue task_queue;
pthread_mutex_t mutexQueue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condQueue = PTHREAD_COND_INITIALIZER;

void *worker(void *arg) {
    task task;

    printf("[INFO] spawned worker thread %d\n", (pid_t) syscall(SYS_gettid));

    while (1) {
        task.routine = NULL;

        pthread_mutex_lock(&mutexQueue);
        while (task_queue.front == -1)
            pthread_cond_wait(&condQueue, &mutexQueue);
        dequeue_task(&task_queue, &task);
        pthread_mutex_unlock(&mutexQueue);

        // printf("[INFO] thread %d running task\n", (pid_t) syscall(SYS_gettid));
        task.routine(task.args);
        // printf("[INFO] thread %d finished task execution\n", (pid_t) syscall(SYS_gettid));
    }

    pthread_exit(0);
}

int submit_task(queue *q, void* (*start_routine)(void *), void *args) {
    int inserted = -1;
    pthread_mutex_lock(&mutexQueue);
    inserted = enqueue_task(q, start_routine, args);
    pthread_mutex_unlock(&mutexQueue);
    if (inserted == 0)
        pthread_cond_signal(&condQueue);
    return inserted;
}

void *countPrimes(void *args) {
    usleep(50000);
    int n = *(int*)args;

    int count = n - 1;

    for (int i = 2; i <= n; i++) {
        for (int j = 2; j <= sqrt(i); j++) {
            if (i % j == 0) {
                count--;
                break;
            }
        }
    }

    printf("(%d) count from 2 to %d: %d\n", (pid_t) syscall(SYS_gettid), n, count);

    free(args);
    // pthread_exit(0);
    return NULL;
}

int main(void) {
    pthread_t th[WORKER_COUNT];

    queue_init(&task_queue);

    for (int i = 0; i < WORKER_COUNT; i++)
        pthread_create(th + i, NULL, worker, NULL);

    for (int i = 0; i < 256; i++) {
        int *args = malloc(sizeof(int));
        *args = 100 + 20 * i;
        if (submit_task(&task_queue, countPrimes, args) < 0) {
            printf("failed to insert task\n");
            free(args);
        }
    } 

    for (int i = 0; i < WORKER_COUNT; i++) 
        pthread_join(th[i], NULL);

    return 0;
}

// Queue

void queue_init(queue *q) {
    q->tasks = malloc(sizeof(task) * QUEUE_CAPACITY);
    q->capacity = QUEUE_CAPACITY;
    q->back = -1;
    q->front = -1;
}

void queue_destroy(queue *q) {
    free(q->tasks);
    memset(q, 0, sizeof(queue));
}

int enqueue_task(queue *q, void* (*start_routine)(void *), void *args) {
    // queue full
    if ((q->back + 1) % q->capacity == q->front)
        return -1;

    q->back = (q->back + 1) % q->capacity;
    q->tasks[q->back].routine = start_routine;
    q->tasks[q->back].args = args;

    if (q->front == -1)
        q->front = q->back;

    return 0;
}

int dequeue_task(queue *q, task *task) {
    if (q->front == -1)
        return -1;

    if (task) {
        task->routine = q->tasks[q->front].routine;
        task->args = q->tasks[q->front].args;
    }

    q->front = (q->front + 1) % q->capacity;
    if (q->front == (q->back + 1) % q->capacity)
        q->back = q->front = -1;

    return 0;
}