// 2023-06-25 15:21
#include <assert.h>

int main(int argc, char *argv[]) {
    float x = 1.0;
    float y = 1.0;
    float z = x + y;
    double a = z;
    a += 0.1;
    float b = fabs(a - 0.3);
    int k = b;
    return 0;
}
