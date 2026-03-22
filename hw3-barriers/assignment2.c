#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 10

pthread_barrier_t barrier;

void *player(void *arg) {
    int id = *(int *)arg;

    int t = rand() % 5 + 1;
    printf("Player %d is getting ready for %d seconds...\n", id + 1, t);
    sleep(t);

    printf("Player %d is ready\n", id + 1);

    pthread_barrier_wait(&barrier);

    printf("Player %d: Game Started!\n", id + 1);

    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];

    srand(time(NULL));

    pthread_barrier_init(&barrier, NULL, N);

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, player, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
