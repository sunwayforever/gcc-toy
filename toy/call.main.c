// 2023-06-24 21:33
#include <assert.h>
extern int x;
extern void foo(int a, int b);
extern int bar(int a, int b);
extern int test();
int main(int argc, char *argv[]) {
    foo(1, 2);
    assert(x == 3);
    assert(bar(1, 2) == 3);
    assert(test() == 4);
}
