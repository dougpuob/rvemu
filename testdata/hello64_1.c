#include <stdio.h>
#include <stdint.h>

int add_new_key(int key, uint64_t val) {
    printf("key: %d\tval: %d\n", key, val);
    return 0;
}

int main() {
    int ret = 0;
    printf("%s\n", __FILE__);
    ret = add_new_key(100, UINT32_MAX + 100);
    ret = add_new_key(200, UINT32_MAX + 300);

    return 0;
}
