// 2023-06-23 17:03
int a = 1;
int b = 1;
int c = 1;
int d = 1;
int e = 1;
int f = 2;
void foo() {
    if (a < 1) {
        a += 1;
    }
    if (b == 1) {
        b += 1;
    }
    if (c >= 1) {
        c += 1;
    }
    if (d <= 1) {
        d += 1;
    }
    if (e != 1) {
        e += 1;
    }
    if (f > 1) {
        f += 1;
    }
}

#include <assert.h>

int main(int argc, char *argv[]) {
    foo();
    assert(a == 1);
    assert(b == 2);
    assert(c == 2);
    assert(d == 2);
    assert(e == 1);
    assert(f == 3);
}
