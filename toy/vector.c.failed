// 2023-07-06 16:04
#include <assert.h>
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

int main(int argc, char *argv[]) {
    v8hi a = {1, 1, 1, 1, 1, 1, 1, 1};
    v8hi b = a + 1;
    for (int i = 0; i < 8; i++) {
        assert(b[i] == 2);
    }
    b -= 1;
    for (int i = 0; i < 8; i++) {
        assert(b[i] == 1);
    }
    b %= 2;
    for (int i = 0; i < 8; i++) {
        assert(b[i] == 1);
    }
    b <<= 3;
    for (int i = 0; i < 8; i++) {
        assert(b[i] == 8);
    }
    v4si c = {1, 1, 1, 1};
    v4si d = c + 1;
    for (int i = 0; i < 4; i++) {
        assert(d[i] == 2);
    }
    return 0;
}
