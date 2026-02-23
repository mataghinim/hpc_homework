#include <stdio.h>

int main(){
    int array[5] = {10, 20, 30, 40, 50};
    int *ptr = array;

    for (int i = 0; i < 5; i++)
        printf("Value of array[%d]: %d\n", i, *(ptr + i));

    int k = 2; // just a multiplier to change the values then
    for (int i = 0; i < 5; i++)
        *(ptr + i) *= k;

    for (int i = 0; i < 5; i++)
        printf("New value of array[%d] (via pointer): %d\n", i, *(ptr + i));

    for (int i = 0; i < 5; i++)
        printf("New value of array[%d] (via array): %d\n", i, array[i]);

    return 0;
};
