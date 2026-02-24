#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 20000000
#define THREADS 4

typedef struct {
    int start;
    int end;   
    long count;      
} ThreadArg;

int is_prime(int x) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if (x % 2 == 0) return 0;

    for (int d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return 0;
    }
    return 1;
}

long sequential_prime_count(int n) {
    long c = 0;
    for (int i = 1; i <= n; i++) {
        if (is_prime(i)) c++;
    }
    return c;
}

void* thread_prime_count(void *arg) {
    ThreadArg *data = (ThreadArg*)arg;

    long c = 0;
    for (int i = (*data).start; i < (*data).end; i++) {
        if (is_prime(i)) c++;
    }

    (*data).count = c;
    return NULL;
}

int main() {
    // sequential
    clock_t t1 = clock();
    long seq = sequential_prime_count(N);
    clock_t t2 = clock();

    // parallel
    pthread_t threads[THREADS];
    ThreadArg args[THREADS];

    int chunk = N / THREADS;
    int rem = N % THREADS;

    clock_t t3 = clock();

    int start = 1;
    for (int i = 0; i < THREADS; i++) {
        int len = chunk + (i < rem ? 1 : 0);

        args[i].start = start;
        args[i].end = start + len;
        args[i].count = 0;

        start += len;

        pthread_create(&threads[i], NULL, thread_prime_count, &args[i]);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    long par = 0;
    for (int i = 0; i < THREADS; i++) {
        par += args[i].count;
    }

    clock_t t4 = clock();

    printf("Sequential primes: %ld\n", seq);
    printf("Parallel primes (%d threads): %ld\n", THREADS, par);

    printf("Sequential time: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    printf("Parallel time:   %f sec\n", (double)(t4 - t3) / CLOCKS_PER_SEC);

    return 0;
}
