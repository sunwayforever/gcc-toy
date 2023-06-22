// 2023-06-22 16:58
int g = 10;
int m[10];
void foo() {
    g = 1;
    int x = g;
    int y = m[2];
    m[2] = 10;
}
