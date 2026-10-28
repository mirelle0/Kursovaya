
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

double calculateIndicator(double X) {
    if (X < 65.0)
        return 6.0 * sqrt(X - 65.0 + 1.0) + 5.0;
    else if (X <= 90.0)
        return 2.0 * (X - 77.5) + 50.0;
    else
        return 0.25 * (X - 90.0) * (X - 90.0) + 55.0;
}

int checkState(double Q) {
    double diff = fabs(Q - 50.0);
    printf("Отклонение от номинала: %.2f\n", diff);
    if (diff > 10.0)
        return 2;
    else
        return 1;
}

int main() {
    setlocale(LC_ALL, "RUS");
    double activityX;
    double resultQ;
    int state;
    printf("Введите X: ");
    scanf("%lf", &activityX);
    resultQ = calculateIndicator(activityX);
    printf("Q = %.4f\n", resultQ);
    state = checkState(resultQ);
    if (state == 1)
        printf("Состояние: НОРМА\n");
    else
        printf("Состояние: ТРЕВОГА\n");
}
