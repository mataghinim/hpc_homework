#include <stdio.h>

int str_length(char *str) {
    char *p = str;
    while (*p != '\0') {
        p++;
    }
    return (int)(p - str);
}

int main() {
    char *literal = "Hello, pointer!";
    char *p = literal;

    while (*p != '\0') {
        printf("%c", *p);
        p++;
    }
    printf("\n");

    char input[200];

    printf("Enter a string: ");
    scanf("%199s", input);

    printf("Length: %d\n", str_length(input));

    return 0;
}

