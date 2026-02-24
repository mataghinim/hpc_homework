#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define N 8
#define ITERATION 100000

void* worker(void *arg) {
    int id = *(int*)arg;
    int dummy = 0;

    for (int i = 0; i < ITERATION; i++) {
        dummy++;  // simple heavy loop
    }

    printf("thread %d ran on cpu %d\n", id, sched_getcpu());
    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
