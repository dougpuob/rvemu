#include <stdio.h>
int main() {
    int n = 100;
    int i = 2;
    int sum = 0;

    printf("Enter a positive integer: ");
    //scanf("%d", &n);

    for (i = 1; i <= n; ++i) {
        sum += i;
    }

    printf("Sum = %d", sum);
    return 0;
}

