import numpy as np
import matplotlib.pyplot as plt
from math import sin, cos, pi, exp


def maximum_norm(vector):
    m = 0
    for x in vector:
        if abs(x) > m:
            m = abs(x)
    return m


def newton_polynomial(x, n, factors, points):
    result = 0
    for i in range(n):
        result += factors[i]
        for j in range(i + 1, n):
            factors[j] *= (x - points[i][0])
    return result


def newton_polynomial_for_domain(domain, n, points):
    result = []
    difference = []
    divided_differences_table = []

    # obliczenie tablicy roznic dzielonych, zaczynamy od kolumny wypelnionej wartosciami w wezlach
    column_0 = [point[1] for point in points]
    divided_differences_table.append(column_0)

    # obliczenie kolejnych kolumn tablicy roznic dzielonych
    for i in range(1, n):
        column = []
        for j in range(n - i):
            value = divided_differences_table[i - 1][j + 1]
            value -= divided_differences_table[i - 1][j]
            value /= (points[j + i][0] - points[j][0])
            column.append(value)
        divided_differences_table.append(column)

    # wspolczynniki to pierwsze wartosci w kazdej z kolumn tablicy roznic dzielonych
    factors = [column[0] for column in divided_differences_table]

    for x in domain:
        result.append(newton_polynomial(x, n, factors.copy(), points))
        difference.append(f(x) - result[-1])

    print("norma maksimum roznicy wielomianu i funkcji (newton): " + str(maximum_norm(difference)))
    return result


def lagrange_polynomial(x, n, points):
    result = 0
    for i in range(n):
        l_factor = 1
        for j in range(n):
            if i != j:
                l_factor = l_factor * (x - points[j][0]) / (points[i][0] - points[j][0])
        result += points[i][1] * l_factor
    return result


def lagrange_polynomial_for_domain(domain, n, points):
    result = []
    difference = []
    for x in domain:
        result.append(lagrange_polynomial(x, n, points))
        difference.append(f(x) - result[-1])

    print("norma maksimum roznicy wielomianu i funkcji (legrange): " + str(maximum_norm(difference)))
    return result


def f(x):
    return sin(4 * x / pi) * exp(-0.2 * x / pi)


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
        plt.subplots_adjust(hspace=0.5)

        plt.subplot(211)
        plt.grid(True)
        plt.plot(domain, lagrange_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Lagrange\'a')

        plt.subplot(212)
        plt.grid(True)
        plt.plot(domain, newton_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Newtona\'a')

        points = []
        for i in range(1, n + 1):
            x = 0.5 * (a + b) + 0.5 * (b - a) * cos((2 * i - 1) * pi / (2 * n))
            points.append((x, f(x)))

        print('\n\nWezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czebyszewa')

        fig = plt.figure(2)
        fig.canvas.set_window_title('Wezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czebyszewa')
        plt.subplots_adjust(hspace=0.5)

        plt.subplot(211)
        plt.grid(True)
        plt.plot(domain, lagrange_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Lagrange\'a')

        plt.subplot(212)
        plt.grid(True)
        plt.plot(domain, newton_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Newtona\'a')

        plt.show()


if __name__ == '__main__':
    main()
