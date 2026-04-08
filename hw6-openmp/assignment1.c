#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000
#define BINS 256

int main() {
    int *A = malloc(N * sizeof(int));
    int hist_naive[BINS] = {0};
    int hist_critical[BINS] = {0};
    int hist_reduction[BINS] = {0};

    for (int i = 0; i < N; i++) {
        A[i] = rand() % 256;
    }

    // 1. Naive parallel version
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        hist_naive[A[i]]++;
    }

    // 2. Version with critical
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp critical
        {
            hist_critical[A[i]]++;
        }
    }

    // 3. Version with reduction
    #pragma omp parallel for reduction(+:hist_reduction[:BINS])
    for (int i = 0; i < N; i++) {
        hist_reduction[A[i]]++;
    }

    //just to see some results
    for (int i = 0; i < 10; i++) {
    printf("bin %d -> naive: %d, critical: %d, reduction: %d\n",
           i, hist_naive[i], hist_critical[i], hist_reduction[i]);
    }

    printf("Done\n");

    free(A);
    return 0;
}
