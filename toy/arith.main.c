// 2023-06-24 18:34
#include <assert.h>
extern int a;
extern int b;
extern int c;
extern void foo();
int main(int argc, char *argv[]) {
    foo();
    assert(a == 4);
    assert(b == 136);
    assert(c == 2);
    return 0;
}
