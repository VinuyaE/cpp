#include <stdio.h>

int bacteria_count(int initial_count, int hours) {
    return initial_count * (1 << hours);
}

int main() {
    int initial_count = 5;
    int hours = 2;

    int total_bacteria = bacteria_count(initial_count, hours);
    printf("The number of bacteria after %d hours is: %d\n", hours, total_bacteria);

    return 0;
}

