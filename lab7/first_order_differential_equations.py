import numpy as np
import matplotlib.pyplot as plt
import sys
from math import sin, cos, pi, exp

# parametry zadania
m = 2
k = 4


def maximum_norm(vector):
    maximum = 0
    for x in vector:
        if abs(x) > maximum:
            maximum = abs(x)
    return maximum


def y_derivative(x, y):
    result = k * k * m * sin(m * x) * cos(m * x)
    result += k * m * y * sin(m * x)

    return result


def solution(x):
    result = exp((-1) * k * cos(m * x))
    result -= k * cos(m * x)
    result += 1

    return result


def solution_for_domain(domain):
    result = []
    for x in domain:
        result.append(solution(x))
    return result


def euler_method(x, points, n):
    i = 0
    while x > points[i + 1][0]:
        if i + 1 < n:
            i += 1
        else:
            break

    a = (points[i + 1][1] - points[i][1]) / (points[i + 1][0] - points[i][0])
    b = points[i][1] - a * points[i][0]
    result = a * x + b

    return result


def euler_method_for_domain(domain, n, x_0, x_k, a):
    result = []
    difference = []

    step = (x_k - x_0) / n

    points = [(x_0, a)]
    x = x_0
    y = a
    for i in range(n):
        y = y + step * y_derivative(x, y)
        x = x + step
        points.append((x, y))

    for point in domain:
        result.append(euler_method(point, points, n))
        difference.append(solution(point) - result[-1])

    print("norma maksimum roznicy dokladnego rozwiazania i rozwiazania wyliczonego metoda Eulera\n"
          + "dla kroku rownego " + str(step) + " (podzial przedzialu na " + str(n) + " czesci):\n"
          + str(maximum_norm(difference)) + "\n")
    return result


def runge_kutta_method(x, points, n):
    i = 0
    while x > points[i + 1][0]:
        if i + 1 < n:
            i += 1
        else:
            break

    a = (points[i + 1][1] - points[i][1]) / (points[i + 1][0] - points[i][0])
    b = points[i][1] - a * points[i][0]
    result = a * x + b

    return result


def runge_kutta_method_for_domain(domain, n, x_0, x_k, a):
    result = []
    difference = []

    step = (x_k - x_0) / n

    points = [(x_0, a)]
    x = x_0
    y = a
    for i in range(n):
        k1 = step * y_derivative(x, y)
        k2 = step * y_derivative(x + step / 2, y + k1 / 2)
        k3 = step * y_derivative(x + step / 2, y + k2 / 2)
        k4 = step * y_derivative(x + step, y + k3)
        y = y + 1 / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
        x = x + step
        points.append((x, y))

    for point in domain:
        result.append(euler_method(point, points, n))
        difference.append(solution(point) - result[-1])

    print("norma maksimum roznicy dokladnego rozwiazania i rozwiazania wyliczonego metoda Rungego-Kutty 4. stopnia\n"
          + "dla kroku rownego " + str(step) + " (podzial przedzialu na " + str(n) + " czesci):\n"
          + str(maximum_norm(difference)))
    return result


def main():
    # liczba krokow na przedziale [x_0, x_k]
    n = 50

    # parametry zadania, przedzial to [x_0, x_k], y(x_0) = a
    x_0 = (-1) * pi / 4
    x_k = 3 * pi / 2
    a = solution(x_0)

    domain = np.linspace(x_0, x_k, num=1500)

    fig = plt.figure(1)
    fig.canvas.set_window_title('Rozwiazania rownania rozniczkowego otrzymane roznymi metodami')
    plt.subplots_adjust(hspace=0.5)

    plt.subplot(211)
    plt.grid(True)
    plt.grid(True)
    plt.plot(domain, euler_method_for_domain(domain, n, x_0, x_k, a), 'k-')
    plt.plot(domain, solution_for_domain(domain), 'b--')
    plt.title('y(x) uzyskane za pomoca metody Eulera')

    plt.subplot(212)
    plt.grid(True)
    plt.plot(domain, runge_kutta_method_for_domain(domain, n, x_0, x_k, a), 'k-')
    plt.plot(domain, solution_for_domain(domain), 'b--')
    plt.title('y(x) uzyskane za pomoca metody Rungego-Kutty 4. stopnia')

    plt.show()


if __name__ == '__main__':
    main()
