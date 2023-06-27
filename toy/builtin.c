// 2023-06-27 17:27
#include <stdio.h>

int main(int argc, char *argv[]) {
    void *ptr = __builtin_getsp();
    printf("%p\n", ptr);
    return 0;
}
