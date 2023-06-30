// 2023-06-30 12:01
#include <assert.h>
float foo1(float x) { return x + 0.1f; }
double foo2(double x) { return x + 0.1; }
int main(int argc, char *argv[]) {
    float x = foo1(foo1(0.1f));
    double y = foo2(foo2(0.1));
    assert(fabs(x - 0.3f) < 0.01);
    assert(fabs(y - 0.3) < 0.01);
    return 0;
}
