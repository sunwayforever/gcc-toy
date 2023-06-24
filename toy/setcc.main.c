// 2023-06-24 18:34
#include <assert.h>
extern int a, b, c, d, e, f;
extern void foo();
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
