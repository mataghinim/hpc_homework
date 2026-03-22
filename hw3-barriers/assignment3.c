#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define S 12

float temps[S];
pthread_barrier_t barrier;
pthread_mutex_t mutex;

void *sensor(void *arg) {
    int id = *(int *)arg;

    float temp = (rand() % 350) / 10.0;
    
    pthread_mutex_lock(&mutex);
    temps[id] = temp;
    printf("Sensor %d collected temperature: %.1f\n", id + 1, temp);
    pthread_mutex_unlock(&mutex);

    pthread_barrier_wait(&barrier);

    if (id == 0) {
        float sum = 0;
        for (int i = 0; i < S; i++) {
            sum += temps[i];
        }
        float avg = sum / S;
        printf("\nAverage temperature: %.2f\n", avg);
    }

    return NULL;
}

int main() {

    pthread_t threads[S];
    int ids[S];

    srand(time(NULL));

    pthread_barrier_init(&barrier, NULL, S);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < S; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, sensor, &ids[i]);
    }

    for (int i = 0; i < S; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);

    return 0;
}
