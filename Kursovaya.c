#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

double calculateIndicator(double activity) {
    int group = 0;

    if (activity < 65.0) group = 1;
    else if (activity <= 90.0) group = 2;
    else group = 3;

    switch (group) {
    case 1: return 6.0 * sqrt(activity - 65.0) + 5.0;
    case 2: return 2.0 * (activity - 77.5) + 50.0;
    case 3: return 0.25 * (activity - 90.0) * (activity - 90.0) + 55.0;
    default: return 0.0;
    }
}

int main() {
    setlocale(LC_ALL, "RUS");

    double activityX;

    printf("¬ведите X: ");
    scanf("%lf", &activityX);

    double resultQ = calculateIndicator(activityX);

    printf("Q = %.4f\n", resultQ);

    return 0;
}