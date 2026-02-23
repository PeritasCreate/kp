#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

#define MAX_ITERATIONS 100
#define EPSILON 1e-10
#define SERIES_TERMS 7  // берём ряд только до n = 7

// Кусочная функция f(x) из варианта
double f(double x) {
    if (x < 0) {
        // тут просто тригонометрия
        return sin(x) + cos(x);
    }
    else if (x >= 0 && x < 2) {
        // (x^2 - 1) / (x - 1), но при x = 1 не делим на ноль
        if (fabs(x - 1.0) < EPSILON) {
            return 2.0; // значение по пределу
        }
        return (x * x - 1.0) / (x - 1.0);
    }
    else { // x >= 2
        // суммируем ряд Σ x^(2n) / (2n+1)! при n = 0..7
        double sum = 0.0;

        for (int n = 0; n <= SERIES_TERMS; n++) {
            double numerator = pow(x, 2.0 * n);

            // считаем факториал (2n+1)!
            double denominator = 1.0;
            for (int k = 1; k <= 2 * n + 1; k++) {
                denominator *= k;
            }

            sum += numerator / denominator;
        }
        return sum;
    }
}

// Производная f'(x)
double f_derivative(double x) {
    if (x < 0) {
        // d/dx[sin(x) + cos(x)] = cos(x) - sin(x)
        return cos(x) - sin(x);
    }
    else if (x >= 0 && x < 2) {
        // на этом кусочке функция равна x + 1, значит производная = 1
        if (fabs(x - 1.0) < EPSILON) {
            return 1.0;
        }
        // тут оставлен вариант через формулу производной дроби
        return (2 * x * (x - 1) - (x * x - 1)) / ((x - 1) * (x - 1));
    }
    else { // x >= 2
        // дифференцируем сумму по членам
        double sum = 0.0;

        for (int n = 1; n <= SERIES_TERMS; n++) {
            double numerator = 2.0 * n * pow(x, 2.0 * n - 1.0);

            double denominator = 1.0;
            for (int k = 1; k <= 2 * n + 1; k++) {
                denominator *= k;
            }

            sum += numerator / denominator;
        }
        return sum;
    }
}

// проверка, что x не равен 1 (там разрыв)
int check_domain(double x) {
    if (fabs(x - 1.0) < EPSILON) {
        return 0;
    }
    return 1;
}

// 1. f(x) в точке
void calculate_at_point() {
    double x;
    printf("\n=== Вычисление f(x) в точке ===\n");
    printf("Введите значение x: ");
    if (scanf("%lf", &x) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    if (!check_domain(x)) {
        printf("Функция не определена в точке x = %.10f\n", x);
        return;
    }

    printf("f(%.10f) = %.10f\n", x, f(x));
}

// 2. Таблица значений на интервале
void calculate_table() {
    double x_start, x_end, step;
    printf("\n=== Таблица значений функции ===\n");
    printf("Введите начало интервала: ");
    if (scanf("%lf", &x_start) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите конец интервала: ");
    if (scanf("%lf", &x_end) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите шаг: ");
    if (scanf("%lf", &step) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    if (step <= 0) {
        printf("Шаг должен быть положительным!\n");
        return;
    }

    if (x_start >= x_end) {
        printf("Начало должно быть меньше конца интервала!\n");
        return;
    }

    printf("\n%15s | %15s\n", "x", "f(x)");
    printf("----------------+-----------------\n");

    for (double x = x_start; x <= x_end; x += step) {
        if (check_domain(x)) {
            printf("%15.6f | %15.10f\n", x, f(x));
        }
        else {
            printf("%15.6f | %15s\n", x, "не определена");
        }
    }
}

// 3. Определённый интеграл методом трапеций
void calculate_integral() {
    double a, b;
    int n;

    printf("\n=== Определенный интеграл методом трапеций ===\n");
    printf("Введите нижний предел интегрирования: ");
    if (scanf("%lf", &a) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите верхний предел интегрирования: ");
    if (scanf("%lf", &b) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите число разбиений (рекомендуется 1000-10000): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Ошибка ввода! Число разбиений должно быть положительным.\n");
        while (getchar() != '\n');
        return;
    }

    if (a >= b) {
        printf("Нижний предел должен быть меньше верхнего!\n");
        return;
    }

    // если x = 1 попадает в интервал, просто предупреждаем
    if (a <= 1.0 && b >= 1.0) {
        printf("Внимание: точка x = 1 находится в интервале интегрирования!\n");
        printf("Функция не определена в этой точке. Интеграл может быть некорректным.\n");
    }

    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (check_domain(x)) {
            sum += f(x);
        }
    }

    double integral = h * sum;
    printf("\nИнтеграл от %.6f до %.6f = %.10f\n", a, b, integral);
    printf("Число разбиений: %d\n", n);
}

// 4. Поиск корня уравнения f(x) = Y (метод Ньютона)
void find_root() {
    double Y, x0, tolerance;

    printf("\n=== Поиск корня уравнения f(x) = Y ===\n");
    printf("Введите значение Y: ");
    if (scanf("%lf", &Y) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите начальное приближение x0: ");
    if (scanf("%lf", &x0) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите точность (например, 1e-6): ");
    if (scanf("%lf", &tolerance) != 1 || tolerance <= 0) {
        printf("Ошибка ввода! Точность должна быть положительной.\n");
        while (getchar() != '\n');
        return;
    }

    if (!check_domain(x0)) {
        printf("Начальное приближение находится вне области определения!\n");
        return;
    }

    double x = x0;
    int iteration = 0;

    printf("\n%5s | %15s | %15s\n", "Итер.", "x", "f(x)");
    printf("------+-----------------+-----------------\n");

    for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        double fx = f(x) - Y;
        double dfx = f_derivative(x);

        printf("%5d | %15.10f | %15.10f\n", iteration, x, f(x));

        if (fabs(fx) < tolerance) {
            printf("\nКорень найден: x = %.10f\n", x);
            printf("f(x) = %.10f\n", f(x));
            printf("Погрешность: %.2e\n", fabs(f(x) - Y));
            printf("Число итераций: %d\n", iteration);
            return;
        }

        if (fabs(dfx) < EPSILON) {
            printf("\nПроизводная близка к нулю. Метод не сходится.\n");
            return;
        }

        x = x - fx / dfx;

        if (!check_domain(x)) {
            printf("\nВышли за область определения функции!\n");
            return;
        }
    }

    printf("\nМетод не сошелся за %d итераций.\n", MAX_ITERATIONS);
    printf("Последнее приближение: x = %.10f, f(x) = %.10f\n", x, f(x));
}

// 5. Значение производной в одной точке
void calculate_derivative() {
    double x;
    printf("\n=== Вычисление производной f'(x) в точке ===\n");
    printf("Введите значение x: ");
    if (scanf("%lf", &x) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }

    if (!check_domain(x)) {
        printf("Функция не определена в точке x = %.10f\n", x);
        return;
    }

    printf("f'(%.10f) = %.10f\n", x, f_derivative(x));
}

// Главное меню
void print_menu() {
    printf("\n========================================\n");
    printf("        МАТЕМАТИЧЕСКИЙ КАЛЬКУЛЯТОР\n");
    printf("========================================\n");
    printf("Функция:\n");
    printf("         | sin(x) + cos(x),             x < 0\n");
    printf("  f(x) = | (x²-1)/(x-1),                0 <= x < 2\n");
    printf("         | Σ(n=0 to 7) x^(2n)/(2n+1)!,  x => 2\n");
    printf("========================================\n");
    printf("1. Вычислить f(x) в точке\n");
    printf("2. Построить таблицу значений\n");
    printf("3. Вычислить определенный интеграл\n");
    printf("4. Найти корень уравнения f(x) = Y\n");
    printf("5. Вычислить производную f'(x)\n");
    printf("0. Выход\n");
    printf("========================================\n");
    printf("Выберите операцию: ");
}

int main() {
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");

    int choice;

    printf("=================================================================\n");
    printf("|  Курсовая работа: Конструирование программы анализа функции   |\n");
    printf("=================================================================\n");

    while (1) {
        print_menu();

        if (scanf("%d", &choice) != 1) {
            printf("\nОшибка ввода! Введите число от 0 до 5.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            calculate_at_point();
            break;
        case 2:
            calculate_table();
            break;
        case 3:
            calculate_integral();
            break;
        case 4:
            find_root();
            break;
        case 5:
            calculate_derivative();
            break;
        case 0:
            printf("\nРабота завершена. До свидания!\n");
            return 0;
        default:
            printf("\nНеверный выбор! Попробуйте снова.\n");
        }

        printf("\nНажмите Enter для продолжения...");
        while (getchar() != '\n');
        getchar();
    }

    return 0;
}
