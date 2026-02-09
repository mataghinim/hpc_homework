#include <stdio.h>
int main(){
    int x = 5;
    int *ptr = &x;
    int **ptd = &ptr;

    printf("Value of x via pointer: %d\n", *ptr);
    printf("Value of x via double pointer: %d\n", **ptd);

    return 0;
}
