import numpy as np
import matplotlib.pyplot as plt
import sys
from math import cos, pi


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


def f(domain, n, points):
    result = []
    for x in domain:
        result += [lagrange_polynomial(x, n, points)]
    return result


def g(domain, n, points):
    result = []
    factors = calculate_factors(n, points)
    for x in domain:
        result += [newton_polynomial(x, n, points, factors.copy())]
    return result


def main():
    # points = []
    # # n = int(input('wpisz liczbe znanych punktow: '))
    # n = 31
    # for i in range(n):
    #     #     print('wpisz x_%d i f(x_%d)' % (i, i))
    #     #     points += [(float(input()), float(input()))]
    #     points += [(float(i - n / 2), float((i - n / 2) ** 3))]
    # # print('punty to: ' + str(points))

    # n = 5
    # points = [(13.0, 123.2), (41.2, -21.9), (100.101, 100.101), (102.1, -1337.7), (137.1337, 420.69)]

    print('wpisz typ rozmieszczenia wezlow')
    print('rownomiernie w calym przedziale - 1')
    print('zgodnie z zerami wielomianu Czebyszewa - 2')
    type_of_nodes = int(input())

    points = []
    if type_of_nodes == 1:
        n = int(input('podaj liczbe punktow '))
        a = float(input('podaj lewy koniec przedzialu '))
        b = float(input('podaj prawy koniec przedzialu '))
        step = (b - a) / (n - 1)
        for i in range(n):
            x = a + i * step
            points += [(x, float(input('podaj f(x) dla ' + str(x) + ' ')))]

    elif type_of_nodes == 2:
        n = int(input('podaj liczbe punktow '))
        for i in range(1, n + 1):
            x = cos((2 * i - 1) * pi / (2 * n))
            points += [(x, float(input('podaj f(x) dla ' + str(x) + ' ')))]

    else:
        sys.exit('podano zla liczbe')

    domain = np.linspace(points[0][0], points[-1][0], num=1000)

    plt.figure(1)
    plt.plot(domain, f(domain, n, points), 'k-')
    plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
    plt.title('Wizualizacja interpolowanego wielomianu Lagrange\'a')

    plt.figure(2)
    plt.plot(domain, g(domain, n, points), 'k-')
    plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
    plt.title('Wizualizacja interpolowanego wielomianu Newtona\'a')

    plt.show()


if __name__ == '__main__':
    main()
