// 2023-06-24 18:34
#include <assert.h>
extern void foo();
extern int a, b, c, d, e, f;
int main(int argc, char *argv[]) {
    foo();
    assert(a == 1);
    assert(b == 2);
    assert(c == 2);
    assert(d == 2);
    assert(e == 1);
    assert(f == 3);
}
