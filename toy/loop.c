// 2023-09-18 11:04
#include <assert.h>

#define N 100
int main(int argc, char *argv[]) {
    int a[N] = {0};

    int i = 0;
    do {
        a[i] += 1;
        i += 1;
    } while (i < N);

    for (int i = 0; i < N; i++) {
        assert(a[i] == 1);
    }

    return 0;
}
