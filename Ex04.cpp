#include <stdio.h>

int sum_of_digits(int n) {
    if (n == 0)
        return 0;
    else
        return (n % 10) + sum_of_digits(n / 10);
}

int main() {
    int number;
    printf("Enter Number: ");
    scanf("%d", &number);

    int sum = sum_of_digits(number);
    printf("Sum of %d is: %d\n", number, sum);

    return 0;
}

