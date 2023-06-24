// 2023-06-24 18:32
#include <assert.h>
extern int g;
extern int m[10];
extern void foo();
int main(int argc, char *argv[]) {
    foo();
    assert(g == 1);
    assert(m[2] == 2);
    return 0;
}
