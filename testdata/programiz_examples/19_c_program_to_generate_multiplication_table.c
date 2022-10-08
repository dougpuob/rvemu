#include <stdio.h>
int main() {
  int n = 20;
  int i = 0;
  printf("Enter an integer: ");
  //scanf("%d", &n);
  for (i = 1; i <= 10; ++i) {
    printf("%d * %d = %d \n", n, i, n * i);
  }
  return 0;
}
