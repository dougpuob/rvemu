#include <stdio.h>
int main() {
    double a = 100;
    double b = 200;
    double product = 0;
    printf("Enter two numbers: ");
    //scanf("%lf %lf", &a, &b);

    // Calculating product
    product = a * b;

    // %.2lf displays number up to 2 decimal point
    printf("Product = %.2lf", product);

    return 0;
}
