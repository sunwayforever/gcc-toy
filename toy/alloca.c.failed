// 2023-07-05 18:24
#include <assert.h>

void foo(int n) {
    char *p = alloca(n);
    for (int i = 0; i < n; i++) {
        p[i] = 2;
    }
    int x = 0;
    int y = 0;
    for (int i = 0; i < n; i++) {
        assert(p[i] == 2);
    }
}

int main(int argc, char *argv[]) {
    char *p = alloca(10);
    for (int i = 0; i < 10; i++) {
        p[i] = 1;
    }
    foo(10);
    for (int i = 0; i < 10; i++) {
        assert(p[i] == 1);
    }
    return 0;
}
