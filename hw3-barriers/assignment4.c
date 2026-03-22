#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define M 5

pthread_barrier_t barrier;

void *worker(void *arg) {
    int id = *(int *)arg;

    printf("Thread %d: Stage 1 started\n", id + 1);
    sleep(1);
    printf("Thread %d: Stage 1 finished\n", id + 1);
    pthread_barrier_wait(&barrier);

    printf("Thread %d: Stage 2 started\n", id + 1);
    sleep(1);
    printf("Thread %d: Stage 2 finished\n", id + 1);
    pthread_barrier_wait(&barrier);

    printf("Thread %d: Stage 3 started\n", id + 1);
    sleep(1);
    printf("Thread %d: Stage 3 finished\n", id + 1);
    pthread_barrier_wait(&barrier);

    return NULL;
}

int main() {
    pthread_t threads[M];
    int ids[M];

    pthread_barrier_init(&barrier, NULL, M);

    for (int i = 0; i < M; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
