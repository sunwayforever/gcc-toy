// 2023-06-27 22:41
#include <assert.h>
long long foo() { return 1; }

long long bar() {
    long long x = foo();
    return x + 1;
}

int main(int argc, char *argv[]) {
    long long x = 1;
    long long y = 1;
    long long z = x + y;
    z *= 2;
    z /= 2;
    assert(z == 2);
    assert(bar() == 2);
    return 0;
}
