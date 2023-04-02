#include <stdio.h>

double multiply (int i, double x);

double summationRecursion (int n, double *sum, double x) {
    if (n == 1) {
        *sum += x;
        return x;
    }
    double f = (-1) * summationRecursion(n - 1, sum, x) * multiply(n - 1, x);
    *sum += f;
    return f;
}

double multiply (int i, double x) {
    return (x*x)/((4*i*i)+(2*i));
}

double summation (double x, int n, double *sum) {
    *sum = 0;
    return summationRecursion(n, sum, x);
}

int main() {
    double x;
    printf("Input x:\n");
    scanf("%lf" , &x);
    int n;
    printf("Input n:\n");
    scanf("%d" , &n);
    double sum;
    summation(x, n, &sum);
    printf("Second program output: %.30lf", sum);
    return 0;
}