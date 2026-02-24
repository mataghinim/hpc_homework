#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 500
#define THREADS 5

typedef struct {
    int *arr;                 
    size_t start;             
    size_t end;               
    long long sum;           
    long long *shared_sum;   
    pthread_mutex_t *mtx;
} ThreadArg;

long long sequential_sum(int *arr) { // version 1
    long long sum = 0;
    for (size_t i = 0; i < SIZE; i++) sum += arr[i];
    return sum;
}

void* thread_sum_local(void *arg) { // version 2
    ThreadArg *data = (ThreadArg*)arg;

    long long local_sum = 0;
    for (size_t i = (*data).start; i < (*data).end; i++) {
        local_sum += (*data).arr[i];
    }

    (*data).sum = local_sum;
    return NULL;
}

void* thread_sum_shared_mutex(void *arg) { // version 3
    ThreadArg *data = (ThreadArg*)arg;

    long long local_sum = 0;
    for (size_t i = (*data).start; i < (*data).end; i++) {
        local_sum += (*data).arr[i];
    }

    pthread_mutex_lock((*data).mtx);
    *((*data).shared_sum) += local_sum;
    pthread_mutex_unlock((*data).mtx);

    return NULL;
}

int main() {
    int *arr = malloc(sizeof(int) * SIZE);
    if (arr == NULL) {
        perror("array allocation failed");
        return 1;
    }

    srand(time(NULL));
    for (size_t i = 0; i < SIZE; i++) arr[i] = rand();

    clock_t t1 = clock();
    long long seq = sequential_sum(arr);
    clock_t t2 = clock();

    size_t chunk = SIZE / THREADS;
    size_t rem = SIZE % THREADS;

    // Version 2
    pthread_t threads2[THREADS];
    ThreadArg args2[THREADS];

    clock_t t3 = clock();

    size_t start = 0;
    for (int i = 0; i < THREADS; i++) {
        size_t len = chunk + (i < (int)rem ? 1 : 0);

        args2[i].arr = arr;
        args2[i].start = start;
        args2[i].end = start + len;
        args2[i].sum = 0;

        start += len;

        pthread_create(&threads2[i], NULL, thread_sum_local, &args2[i]);
    }

    for (int i = 0; i < THREADS; i++) pthread_join(threads2[i], NULL);

    long long par_local = 0;
    for (int i = 0; i < THREADS; i++) par_local += args2[i].sum;

    clock_t t4 = clock();

    // Version 3
    pthread_t threads3[THREADS];
    ThreadArg args3[THREADS];
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);

    long long shared_sum = 0;

    clock_t t5 = clock();

    start = 0;
    for (int i = 0; i < THREADS; i++) {
        size_t len = chunk + (i < (int)rem ? 1 : 0);

        args3[i].arr = arr;
        args3[i].start = start;
        args3[i].end = start + len;
        args3[i].shared_sum = &shared_sum;
        args3[i].mtx = &mtx;

        start += len;

        pthread_create(&threads3[i], NULL, thread_sum_shared_mutex, &args3[i]);
    }

    for (int i = 0; i < THREADS; i++) pthread_join(threads3[i], NULL);

    clock_t t6 = clock();

    pthread_mutex_destroy(&mtx);

    printf("Sequential sum: %lld\n", seq);
    printf("Parallel sum (local sums): %lld\n", par_local);
    printf("Parallel sum (shared+mutex): %lld\n", shared_sum);

    printf("Sequential time: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    printf("Parallel time (local sums): %f sec\n", (double)(t4 - t3) / CLOCKS_PER_SEC);
    printf("Parallel time (shared+mutex): %f sec\n", (double)(t6 - t5) / CLOCKS_PER_SEC);

    free(arr);
    return 0;
}
