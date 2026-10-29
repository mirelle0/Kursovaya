#include <stdio.h>
#include <locale.h>
#include <math.h>

#define Q_NOMINAL 50.0
#define Q_THRESHOLD 10.0
#define X_BOUND_LOW 65.0
#define X_BOUND_HIGH 90.0
#define X_MIN 0.0
#define X_MAX 100.0
#define X_MID 77.5

double read_x(void);
double calculate_q(double x);
int check_state(double q);
void print_report(void);

int main(void) {
    setlocale(LC_ALL, "RUS");
    double x;
    double q;
    double deviation;
    int state;
    printf("Программа мониторинга активности катализатора реактора.\n");
    printf("Введите значение X для расчёта показателя Q и оценки состояния.\n\n");
    printf("Введите X (от 0 до 100): ");
    x = read_x();
    if (x < X_MIN || x > X_MAX) {
        printf("Ошибка: X должен находиться в диапазоне от 0 до 100.\n");
        return 1;
    }
    q = calculate_q(x);
    deviation = fabs(q - Q_NOMINAL);
    state = check_state(q);
    printf("Показатель Q = %.2f\n", q);
    printf("Отклонение от номинала = %.2f\n", deviation);
    if (state == 0)
        printf("Состояние: НОРМА\n");
    else
        printf("Состояние: ТРЕВОГА\n");
}

double read_x(void) {
    double x;
    scanf_s("%lf", &x);
    return x;
}

double calculate_q(double x) {
    if (x < X_BOUND_LOW)
        return 6.0 * sqrt(x - X_BOUND_LOW + 1.0) + 5.0;
    else if (x <= X_BOUND_HIGH)
        return 2.0 * (x - X_MID) + Q_NOMINAL;
    else
        return 0.25 * (x - X_BOUND_HIGH) *
        (x - X_BOUND_HIGH) + 55.0;
}

int check_state(double q) {
    if (fabs(q - Q_NOMINAL) > Q_THRESHOLD)
        return 1;
    else
        return 0;
}

void print_report(void) {
}
