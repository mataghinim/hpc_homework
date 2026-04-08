#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000

int main() {
    double *A = malloc(N * sizeof(double));
    double max_val = 0.0;
    double T, sum = 0.0;

    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        A[i] = (double)rand();
    }

    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < N; i++) {
        if (A[i] > max_val) {
            max_val = A[i];
        }
    }

    T = 0.8 * max_val;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        if (A[i] > T) {
            sum += A[i];
        }
    }

    printf("Max = %f\n", max_val);
    printf("Threshold T = %f\n", T);
    printf("Sum = %f\n", sum);

    free(A);
    return 0;
}
