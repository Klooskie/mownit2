import numpy as np
import matplotlib.pyplot as plt
from math import sin, cos, pi, exp


def maximum_norm(vector):
    m = 0
    for x in vector:
        if abs(x) > m:
            m = abs(x)
    return m


def hermite_polynomial(x, n, factors, new_points):
    result = 0
    for i in range(2 * n):
        result += factors[i]
        for j in range(i + 1, 2 * n):
            factors[j] *= (x - new_points[i])
    return result


def hermite_polynomial_for_domain(domain, n, points):
    result = []
    difference = []

    # tworzymy liste nowych iksow - podwajamy wystepowanie iksow wszystkich wezlow 
    new_points = []
    for p in points:
        new_points.append(p[0])
        new_points.append(p[0])

    # tworzymy tablice roznic dzielonych
    divided_differences_table = []

    # pierwsza kolumna tablicy roznic dzielonych (0) - zawiera wartosci funkcji w odpowiednich x
    column_0 = []
    for p in points:
        column_0.append(p[1])
        column_0.append(p[1])

    divided_differences_table.append(column_0)

    # druga kolumna tablicy roznic dzielonych (1) - tylko ona zawiera wartosci pochodnej
    column_1 = []
    for i in range(2 * n - 1):
        if i % 2 == 0:
            column_1.append(f_derivative(new_points[i]))
        else:
            value = divided_differences_table[0][i + 1]
            value -= divided_differences_table[0][i]
            value /= (new_points[i + 1] - new_points[i])
            column_1.append(value)

    divided_differences_table.append(column_1)

    # reszta kolumn tablicy roznic dzielonych
    for i in range(2, 2 * n):
        column = []
        for j in range(2 * n - i):
            value = divided_differences_table[i - 1][j + 1]
            value -= divided_differences_table[i - 1][j]
            value /= (new_points[j + i] - new_points[j])
            column.append(value)
        divided_differences_table.append(column)

    factors = []
    for column in divided_differences_table:
        factors.append(column[0])

    for x in domain:
        result.append(hermite_polynomial(x, n, factors.copy(), new_points))
        difference.append(f(x) - result[-1])

    print("norma maksimum roznicy wielomianu i funkcji (hermite): " + str(maximum_norm(difference)))
    return result


def f(x):
    return sin(4 * x / pi) * exp(-0.2 * x / pi)


def f_derivative(x):
    result = (4 / pi) * cos(4 * x / pi) * exp(-0.2 * x / pi)
    result += sin(4 * x / pi) * (-0.2 / pi) * exp(-0.2 * x / pi)
    return result


def f_for_domain(domain):
    result = []
    for x in domain:
        result.append(f(x))
    return result


def main():
    for n in range(2, 22):
        if n == 21:
            n = 30

        a = -5
        b = 10

        points = []
        step = (b - a) / (n - 1)
        for i in range(n):
            x = a + i * step
            points.append((x, f(x)))

        domain = np.linspace(a, b, num=1500)

        print('\n\nWezly (' + str(n) + ') rozmieszczone rownomiernie na calym przedziale')
        fig = plt.figure(1)
        fig.canvas.set_window_title('Wezly (' + str(n) + ') rozmieszczone rownomiernie na calym przedziale')

        plt.grid(True)
        plt.plot(domain, hermite_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Hermite\'a')

        points = []
        for i in range(1, n + 1):
            x = 0.5 * (a + b) + 0.5 * (b - a) * cos((2 * i - 1) * pi / (2 * n))
            points.append((x, f(x)))

        print('\n\nWezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czebyszewa')
        fig = plt.figure(2)
        fig.canvas.set_window_title('Wezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czebyszewa')

        plt.grid(True)
        plt.plot(domain, hermite_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Hermite\'a')

        plt.show()


if __name__ == '__main__':
    main()
