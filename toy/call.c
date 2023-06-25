// 2023-06-24 20:10

int x = 0;
void foo(int a, int b) { x = a + b; }
int bar(int a, int b) { return a + b; };
int test() {
    int x = bar(1, 2);
    x += 1;
    return x;
}

#include <assert.h>
int main(int argc, char *argv[]) {
    foo(1, 2);
    assert(x == 3);
    assert(bar(1, 2) == 3);
    assert(test() == 4);
}
