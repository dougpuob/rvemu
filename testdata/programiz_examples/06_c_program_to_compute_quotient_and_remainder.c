#include <stdio.h>
int main() {
    int dividend= 10;
    int divisor = 3;
    int quotient = 0;
    int remainder = 0;
    printf("Enter dividend: ");
    //scanf("%d", &dividend);
    printf("Enter divisor: ");
    //scanf("%d", &divisor);

    // Computes quotient
    quotient = dividend / divisor;

    // Computes remainder
    remainder = dividend % divisor;

    printf("Quotient = %d\n", quotient);
    printf("Remainder = %d", remainder);
    return 0;
}
