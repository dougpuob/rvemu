#include <stdio.h>

int add_new_key(int key, int val) {
    printf("key: %d\tval: %d\n", key, val);
    return 0;
}

int main() {
    int ret = 0;
    printf("%s\n", __FILE__);
    ret = add_new_key(100, 200);
    ret = add_new_key(200, 300);
 
    return 0;
}
