#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

double calculateIndicator(double X) {
    int group = 0;
    if (X < 65.0) group = 1;
    else if (X <= 90.0) group = 2;
    else group = 3;
    switch (group) {
    case 1: return 6.0 * sqrt(X - 65.0) + 5.0;
    case 2: return 2.0 * (X - 77.5) + 50.0;
    case 3: return 0.25 * (X - 90.0) * (X - 90.0) + 55.0;
    default: return 0.0;
    }
}

void checkState(double Q) {
    double diff = fabs(Q - 50.0);
    printf("Отклонение от номинала: %.2f\n", diff);

    if (diff <= 10.0) {
        printf("Состояние: НОРМА\n");
    }
    else {
        printf("Состояние: ТРЕВОГА (отклонение больше 10)\n");
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    double activityX;

    printf("Введите X: ");
    scanf("%lf", &activityX);

    double resultQ = calculateIndicator(activityX);
    printf("Q = %.4f\n", resultQ);

    checkState(resultQ); 

    return 0;
}
