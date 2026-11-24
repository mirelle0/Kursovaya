#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#define Q_NOMINAL 50.0
#define Q_THRESHOLD 10.0
#define X_BOUND_LOW 65.0
#define X_BOUND_HIGH 90.0
#define X_MIN 0.0
#define X_MAX 100.0
#define X_MID 77.5
#define N 40
#define LINE_SIZE 100

double read_x(void);
int parse_number(char line[], double* value);
double calculate_q(double x);
int check_state(double q, double threshold);
void print_report(double x, double q, double threshold);
void process_file(double threshold);
void calculate_statistics(double q[]);

int main(void) {
    setlocale(LC_ALL, "RUS");
    setlocale(LC_NUMERIC, "C");
    double x, q, threshold = Q_THRESHOLD;
    double choice;
    do {
        printf("\n--- МОНИТОРИНГ АКТИВНОСТИ КАТАЛИЗАТОРА ---\n");
        printf("1. Ручной ввод\n");
        printf("2. Чтение измерений из файла\n");
        printf("3. Изменить допустимое отклонение\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        choice = read_x();
        if (!isfinite(choice) || choice < 0 ||
            choice > 3 || choice != (int)choice) {
            printf("Ошибка: неверный пункт меню.\n");
            continue;
        }
        if (choice == 1) {
            printf("Введите X (от 0 до 100): ");
            x = read_x();
            if (!isfinite(x) || x < X_MIN || x > X_MAX) {
                printf("Ошибка: некорректное значение X.\n");
                continue;
            }
            q = calculate_q(x);
            if (!isfinite(q)) {
                printf("Невозможно вычислить Q по заданной формуле.\n");
                continue;
            }
            print_report(x, q, threshold);
        }
        else if (choice == 2) {
            process_file(threshold);
        }
        else if (choice == 3) {
            printf("Введите новое допустимое отклонение: ");
            x = read_x();
            if (!isfinite(x) || x <= 0) {
                printf("Ошибка: значение должно быть положительным.\n");
                continue;
            }
            threshold = x;
            printf("Допустимое отклонение изменено: %.2f\n", threshold);
        }
    } while (choice != 0);
    printf("Завершение программы.\n");
    return 0;
}

double read_x(void) {
    char line[LINE_SIZE];
    double x;
    if (fgets(line, LINE_SIZE, stdin) == NULL)
        return NAN;
    if (!parse_number(line, &x))
        return NAN;
    return x;
}

int parse_number(char line[], double* value) {
    char* end;
    double number;
    errno = 0;
    number = strtod(line, &end);
    if (end == line || errno == ERANGE || !isfinite(number))
        return 0;
    while (isspace((unsigned char)*end))
        end++;
    if (*end != '\0')
        return 0;
    *value = number;
    return 1;
}

double calculate_q(double x) {
    if (x < X_BOUND_LOW) {
        if (x - X_BOUND_LOW + 1.0 < 0)
            return NAN;
        return 6.0 * sqrt(x - X_BOUND_LOW + 1.0) + 5.0;
    }
    else if (x <= X_BOUND_HIGH)
        return 2.0 * (x - X_MID) + Q_NOMINAL;
    else
        return 0.25 * (x - X_BOUND_HIGH) *
        (x - X_BOUND_HIGH) + 55.0;
}

int check_state(double q, double threshold) {
    if (fabs(q - Q_NOMINAL) > threshold)
        return 1;
    else
        return 0;
}

void print_report(double x, double q, double threshold) {
    printf("X = %.2f %%\n", x);
    printf("Q = %.2f\n", q);
    printf("Отклонение = %.2f\n", fabs(q - Q_NOMINAL));
    if (check_state(q, threshold) == 0)
        printf("Состояние: НОРМА\n");
    else
        printf("Состояние: ТРЕВОГА\n");
}

void process_file(double threshold) {
    FILE* file;
    char line[LINE_SIZE];
    double x, q;
    double results[15];
    int count = 0;
    int line_number = 0;
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Ошибка открытия файла input.txt.\n");
        return;
    }
    printf("\n--- ОБРАБОТКА ФАЙЛА ---\n");
    while (fgets(line, LINE_SIZE, file) != NULL) {
        line_number++;
        if (strchr(line, '\n') == NULL && !feof(file)) {
            printf("Ошибка: слишком длинная запись в строке %d.\n",
                line_number);
            fclose(file);
            return;
        }
        if (strspn(line, " \t\r\n") == strlen(line))
            continue;
        if (!parse_number(line, &x)) {
            printf("Ошибка: некорректная запись в строке %d.\n",
                line_number);
            fclose(file);
            return;
        }
        if (x < X_MIN || x > X_MAX) {
            printf("Ошибка: X в строке %d выходит за диапазон 0-100.\n",
                line_number);
            fclose(file);
            return;
        }
        if (count >= N) {
            printf("Ошибка: количество измерений превышает 40.\n");
            fclose(file);
            return;
        }
        q = calculate_q(x);
        if (!isfinite(q)) {
            printf("Ошибка вычисления Q в строке %d.\n",
                line_number);
            fclose(file);
            return;
        }
        if (count < 15)
            results[count] = q;
        count++;
        printf("\nИзмерение №%d:\n", count);
        print_report(x, q, threshold);
    }
    if (ferror(file)) {
        printf("Ошибка чтения файла.\n");
        fclose(file);
        return;
    }
    fclose(file);
    if (count == 0) {
        printf("Файл пустой. Измерения отсутствуют.\n");
        return;
    }
    printf("\nОбработка файла завершена.\n");
    printf("Количество обработанных измерений: %d\n", count);
    if (count < N)
        printf("Предупреждение: получено меньше 40 измерений.\n");
    if (count >= 15)
        calculate_statistics(results);
    else
        printf("Недостаточно данных для статистики первых 15 измерений.\n");
}

void calculate_statistics(double q[]) {
    double sum = 0;
    double increase;
    double max = 0;
    double min = 0;
    int i;
    for (i = 0; i < 14; i++) {
        increase = fabs(q[i + 1] - q[i]);

        sum += increase;

        if (i == 0 || increase > max)
            max = increase;

        if (i == 0 || increase < min)
            min = increase;
    }
    printf("\n--- СТАТИСТИЧЕСКИЙ АНАЛИЗ ---\n");
    printf("Средний абсолютный прирост: %.2f\n", sum / 14);
    printf("Максимальный прирост: %.2f\n", max);
    printf("Минимальный прирост: %.2f\n", min);
}
