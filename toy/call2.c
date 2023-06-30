// 2023-06-30 12:01
#include <assert.h>
float foo1(float x) { return x + 0.1f; }
double foo2(double x) { return x + 0.1; }
int main(int argc, char *argv[]) {
    float x = foo1(foo1(0.1f));
    double y = foo2(foo2(0.1));
    assert(fabs(x - 0.3f) < 0.01);
    assert(fabs(y - 0.3) < 0.01);

    float a = 0.1f;
    a = foo1(a) + (foo1(a) + foo1(a));
    float b = 0.1;
    b = foo2(b) + (foo2(b) + foo2(b));
    assert(fabs(a - 0.6f) < 0.01f);
    assert(fabs(b - 0.6) < 0.01);
    return 0;
}
