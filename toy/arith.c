// 2023-06-20 19:24
#include <stdint.h>

int a;
int b;
int c;
int d;
int e;
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

    int8_t t = -1;
    d = t % 3;
    e = (uint8_t)t % 3;
}

#include <assert.h>

int main(int argc, char *argv[]) {
    foo();
    assert(a == 4);
    assert(b == 136);
    assert(c == 2);
    assert(d == -1);
    assert(e == 0);
    return 0;
}
