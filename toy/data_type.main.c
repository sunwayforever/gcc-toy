// 2023-06-25 14:26
#include <assert.h>
extern char foo();
int main(int argc, char *argv[]) {
    assert(foo(2) == 8);
    return 0;
}
