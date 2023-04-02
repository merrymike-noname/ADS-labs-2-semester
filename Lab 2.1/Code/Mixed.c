#include <stdio.h>

double multiply (int i, double x);

double recursionSum (double previousValue, int iteration, double x, int n) {
    double f;
    if (iteration == 1) {
        f = x;
    } else {
        f = (-1) * previousValue * multiply(iteration - 1, x);
    }
    if (iteration == n) {
        return f;
    } else {
        double sum = f + recursionSum(f, iteration + 1, x, n);
        return sum;
    }
}

double multiply (int i, double x) {
    return (x*x)/((4*i*i)+(2*i));
}

double recursion(double x, int n){
    return recursionSum(0,1,x,n);
}

int main() {
    double x;
    printf("Input x:\n");
    scanf("%lf" , &x);
    int n;
    printf("Input n:\n");
    scanf("%d" , &n);
    double sum = recursion(x, n);
    printf("Third program output: %.30lf", sum);
    return 0;
}
