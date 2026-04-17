#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long long fib(int n) {
    if (n <= 1) {
        return n;
    }

    // small values computed sequentially to reduce task overhead
    if (n <= 10) {
        return fib(n - 1) + fib(n - 2);
    }

    long long x, y;

    #pragma omp task shared(x)
    x = fib(n - 1);

    #pragma omp task shared(y)
    y = fib(n - 2);

    #pragma omp taskwait
    return x + y;
}

int main() {
    int num;
    long long result;

    printf("Enter n: ");
    scanf("%d", &num);

    #pragma omp parallel
    {
        #pragma omp single
        {
            result = fib(num);
        }
    }

    printf("F(%d) = %lld\n", num, result);

    return 0;
}