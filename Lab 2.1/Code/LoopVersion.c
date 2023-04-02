#include <stdio.h>

double multiply(int i, double x);

double recursion (double x, int n) {
    double sum = x;
    double f = x;
    for (int i = 2; i <= n; i++) {
        f *= (-1) * multiply(i - 1, x);
        sum += f;
    }
    return sum;
}

double multiply (int i, double x) {
    return (x*x)/((4*i*i)+(2*i));
}

int main() {
    double x;
    printf("Input x:\n");
    scanf("%lf" , &x);
    unsigned int n;
    printf("Input n:\n");
    scanf("%d" , &n);
    double sum = recursion(x, n);
    printf("Test output: %.30lf", sum);
    return 0;
}