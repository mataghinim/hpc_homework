#include <stdio.h>

int main() {
    char str1[] = "apple";
    char str2[] = "banana";
    char str3[] = "cherry";

    char *words[] = {str1, str2, str3};

    for (int i = 0; i < 3; i++) {
        char *p = *(words + i);
        while (*p != '\0') {
            printf("%c", *p);
            p++;
        }
        printf("\n");
    }

    *(words + 1) = "mango";

    printf("\nAfter modification:\n");
    for (int i = 0; i < 3; i++) {
        printf("%s\n", *(words + i));
    }

    return 0;
}

