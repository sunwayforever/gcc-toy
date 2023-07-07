// 2023-07-07 15:02
#include <assert.h>

int main(int argc, char *argv[]) {
    int x = 1;
    int y = __builtin_clz(x);
    assert(y == 31);
}
