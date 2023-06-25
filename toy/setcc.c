// 2023-06-23 10:11
int a, b, c, d, e, f;
void foo() {
    int x = 0;
    int y = 1;
    a = x > y;
    b = x >= y;
    c = x <= y;
    d = x == y;
    e = x != y;
    f = x < y;
}

#include <assert.h>
int main(int argc, char *argv[]) {
    foo();
    assert(a == 0);
    assert(b == 0);
    assert(c == 1);
    assert(d == 0);
    assert(e == 1);
    assert(f == 1);
    return 0;
}
