#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <omp.h>

#define N 50000000

int main() {
    double *A = malloc(N * sizeof(double));
    double min_diff = DBL_MAX;

    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

   
    for (int i = 0; i < N; i++) {
        A[i] = (double)rand();
    }

    #pragma omp parallel for reduction(min:min_diff)
    for (int i = 1; i < N; i++) {
        double diff = fabs(A[i] - A[i - 1]);
        if (diff < min_diff) {
            min_diff = diff;
        }
    }

    printf("min difference = %f\n", min_diff);

    free(A);
    return 0;
}
