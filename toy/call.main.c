// 2023-06-24 21:33
#include <assert.h>
extern int x;
extern void foo();
int main(int argc, char *argv[]) {
    foo(1, 2);
    assert(x == 3);
}
