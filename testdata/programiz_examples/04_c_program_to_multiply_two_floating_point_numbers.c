#include <stdio.h>

#include <stdio.h>
int main() {
    double a = 100.0;
    double b = 200.0;
    double product;
    printf("Enter two numbers: ");
    //scanf("%lf %lf", &a, &b);

    // Calculating product
    product = a * b;

    // %.2lf displays number up to 2 decimal point
    printf("Product = %.2lf", product);

    return 0;
}
