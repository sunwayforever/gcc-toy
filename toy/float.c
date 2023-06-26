// 2023-06-25 15:21
#include <assert.h>

double foo(float m) {
    float x = m;
    float y = 0.1;
    float z = x + y;
    double a = z;
    a += 0.1;
    int k = z;
    float b = fabs(a - 0.3);
    int check = b < 0.01;
    assert(check);
    assert(b <= 0.01);
    assert(b < 0.01);
    assert(b >= 0.0f);
    assert(b > 0.0f);
    assert(b != 0.0f);
    return b;
}

int main(int argc, char *argv[]) {
    assert(foo(0.1f) < 0.01);
    return 0;
}
