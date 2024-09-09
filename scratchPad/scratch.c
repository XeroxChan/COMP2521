#include <stdio.h>

int fib(int num, int sum);

int main() {
    listA = 
}

int fib(int num, int sum){
    
    if (num == 0) {
        return 0;
    }
    if (num == 1) {
        return 1;
    }
    
    sum++;
    printf("%d\n", sum);
    return fib(num - 1, sum) + fib(num -2, sum);
}
