import numpy as np
import matplotlib.pyplot as plt
import sys
from math import sin, cos, pi, exp


def maximum_norm(vector):
    m = 0
    for x in vector:
        if abs(x) > m:
            m = abs(x)
    return m


def lagrange_polynomial(x, n, points):
    result = 0
    for i in range(n):
        l_factor = 1
        for j in range(n):
            if i != j:
                l_factor = l_factor * (x - points[j][0]) / (points[i][0] - points[j][0])
        result += points[i][1] * l_factor
    return result


def newton_polynomial(x, n, points, factors):
    result = 0
    for i in range(n):
        result += factors[i]
        for j in range(i + 1, n):
            factors[j] *= (x - points[i][0])
    return result


def calculate_factors(n, points):
    factors = []
    for i in range(n):
        a_i = points[i][1]
        for j in range(i):
            a_i -= factors[j]
            a_i /= (points[i][0] - points[j][0])
        factors += [a_i]
    return factors


def lagrange_polynomial_for_domain(domain, n, points):
    result = []
    difference = []
    for x in domain:
        result += [lagrange_polynomial(x, n, points)]
        difference += [f(x) - result[-1]]

    print("norma maksimum roznicy wielomianu i funkcji (legrange): " + str(maximum_norm(difference)))
    return result


def newton_polynomial_for_domain(domain, n, points):
    result = []
    difference = []
    factors = calculate_factors(n, points)
    for x in domain:
        result += [newton_polynomial(x, n, points, factors.copy())]
        difference += [f(x) - result[-1]]

    print("norma maksimum roznicy wielomianu i funkcji (newton): " + str(maximum_norm(difference)))
    return result


def f(x):
    return sin(4 * x / pi) * exp(-0.2 * x / pi)


def f_for_domain(domain):
    result = []
    for x in domain:
        result += [f(x)]
    return result


def main():

    print('wpisz typ rozmieszczenia wezlow')
    print('rownomiernie w calym przedziale - 1')
    print('zgodnie z zerami wielomianu Czebyszewa - 2')
    # type_of_nodes = int(input())
    type_of_nodes = 2;

    for n in range(2, 21):

        a = -5
        b = 10

        # points = []
        # if type_of_nodes == 1:
        #     # a = float(input('podaj lewy koniec przedzialu '))
        #     # b = float(input('podaj prawy koniec przedzialu '))
        #     # n = int(input('podaj liczbe wezlow '))
        #     step = (b - a) / (n - 1)
        #     for i in range(n):
        #         x = a + i * step
        #         # points += [(x, float(input('podaj f(x) dla ' + str(x) + ' ')))]
        #         points += [(x, f(x))]
        #
        # elif type_of_nodes == 2:
        #     # a = float(input('podaj lewy koniec przedzialu '))
        #     # b = float(input('podaj prawy koniec przedzialu '))
        #     # n = int(input('podaj liczbe wezlow '))
        #     for i in range(1, n + 1):
        #         x = 0.5 * (a + b) + 0.5 * (b - a) * cos((2 * i - 1) * pi / (2 * n))
        #         # points += [(x, float(input('podaj f(x) dla ' + str(x) + ' ')))]
        #         points += [(x, f(x))]
        #
        # else:
        #     sys.exit('podano zla liczbe')

        points = []
        step = (b - a) / (n - 1)
        for i in range(n):
            x = a + i * step
            points += [(x, f(x))]

        domain = np.linspace(a, b, num=1500)

        print('\n\nWezly (' + str(n) + ') rozmieszczone rownomiernie na calym przedziale')

        fig = plt.figure(1)
        fig.canvas.set_window_title('Wezly (' + str(n) + ') rozmieszczone rownomiernie na calym przedziale')
        plt.subplots_adjust(hspace=0.5)

        plt.subplot(211)
        plt.plot(domain, lagrange_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Lagrange\'a')

        plt.subplot(212)
        plt.plot(domain, newton_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Newtona\'a')

        points = []
        for i in range(1, n + 1):
            x = 0.5 * (a + b) + 0.5 * (b - a) * cos((2 * i - 1) * pi / (2 * n))
            points += [(x, f(x))]

        print('\n\nWezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czybyszewa')

        fig = plt.figure(2)
        fig.canvas.set_window_title('Wezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czybyszewa')
        plt.subplots_adjust(hspace=0.5)

        plt.subplot(211)
        plt.plot(domain, lagrange_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Lagrange\'a')

        plt.subplot(212)
        plt.plot(domain, newton_polynomial_for_domain(domain, n, points), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolowanego wielomianu Newtona\'a')

        plt.show()


if __name__ == '__main__':
    main()
