#include <stdio.h>
#include <stdlib.h>

int main() {
    int *num;
    int *ptr;
    int n = 5;

    num = malloc(sizeof(*num));
    if (num == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    *num = 57;
    printf("Value of allocated integer: %d\n", *num);

    ptr = malloc(n * sizeof(*ptr));
    if (ptr == NULL) {
        printf("Memory allocation failed.\n");
        free(num);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        *(ptr + i) = i + 100;
        printf("ptr[%d] = %d\n", i, *(ptr + i));
    }

    free(num);
    free(ptr);

    return 0;
}

