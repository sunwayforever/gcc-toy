// 2023-06-27 17:27
#include <assert.h>

int main(int argc, char *argv[]) {
    void *ptr = __builtin_getsp();
    void *sp = 0;
    asm("addi %0, sp, 0" : "=r"(sp));
    assert(sp == ptr);
    return 0;
}
