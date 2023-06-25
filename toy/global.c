// 2023-06-22 16:58
int g = 10;
int m[10];
void foo() {
    g = 1;
    int x = g;
    m[2] = 1;
    int y = m[2];
    y += 1;
    m[2] = y;
}

#include <assert.h>
int main(int argc, char *argv[]) {
    foo();
    assert(g == 1);
    assert(m[2] == 2);
    return 0;
}
