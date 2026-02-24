#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 50000000UL
#define THREADS 4

typedef struct {
    long *arr;
    int start;
    int end;      // [start, end)
    long local_max;
} ThreadArg;

long sequential_max(long *arr) {
    long mx = arr[0];
    for (int i = 1; i < (int)SIZE; i++) {
        if (arr[i] > mx) mx = arr[i];
    }
    return mx;
}

void* thread_max(void *arg) {
    ThreadArg *data = (ThreadArg*)arg;

    long mx = (*data).arr[(*data).start];
    for (int i = (*data).start + 1; i < (*data).end; i++) {
        if ((*data).arr[i] > mx) mx = (*data).arr[i];
    }

    (*data).local_max = mx;
    return NULL;
}

int main() {
    long *arr = malloc(sizeof(long) * SIZE);
    if (arr == NULL) {
        perror("array allocation failed");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < (int)SIZE; i++) {
        arr[i] = (long)rand();
    }

    // Sequential
    clock_t t1 = clock();
    long seq_max = sequential_max(arr);
    clock_t t2 = clock();

    // Parallel (4 threads)
    pthread_t threads[THREADS];
    ThreadArg args[THREADS];

    int chunk = (int)(SIZE / THREADS);
    int rem = (int)(SIZE % THREADS);

    clock_t t3 = clock();

    int start = 0;
    for (int i = 0; i < THREADS; i++) {
        int len = chunk + (i < rem ? 1 : 0);

        args[i].arr = arr;
        args[i].start = start;
        args[i].end = start + len;
        args[i].local_max = 0;

        start += len;

        pthread_create(&threads[i], NULL, thread_max, &args[i]);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    long par_max = args[0].local_max;
    for (int i = 1; i < THREADS; i++) {
        if (args[i].local_max > par_max) par_max = args[i].local_max;
    }

    clock_t t4 = clock();

    printf("Sequential max: %ld\n", seq_max);
    printf("Parallel max (%d threads): %ld\n", THREADS, par_max);

    printf("Sequential time: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    printf("Parallel time:   %f sec\n", (double)(t4 - t3) / CLOCKS_PER_SEC);

    free(arr);
    return 0;
}
