// 2023-06-20 19:24
int a;
int b;
int c;
void foo() {
    int x = 1;
    int y = 1;
    x = x + x + y;
    x += 1;
    a = x;

    x *= 102;
    x /= 3;
    b = x;

    x = x & 0x11;
    x = x | 0x12;
    x = x ^ 0x13;
    x = x << 2;
    x = x >> 1;
    c = x;
}

#include <assert.h>

int main(int argc, char *argv[]) {
    foo();
    assert(a == 4);
    assert(b == 136);
    assert(c == 2);
    return 0;
}
