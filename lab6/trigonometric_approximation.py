import numpy as np
import matplotlib.pyplot as plt
from math import sin, cos, pi, exp, sqrt


def maximum_norm(vector):
    m = 0
    for x in vector:
        if abs(x) > m:
            m = abs(x)
    return m


def euclidean_norm(vector):
    sum_of_squares = 0
    for x in vector:
        sum_of_squares += x * x
    sum_of_squares /= len(vector)
    return sqrt(sum_of_squares)


def f(x):
    return sin(4 * x / pi) * exp(-0.2 * x / pi)


def f_for_domain(domain):
    result = []
    for x in domain:
        result.append(f(x))
    return result


def convert_x_to_v(x):
    return (x + 5) * 2 * pi / 15


def trigonometric_approximation(x, a_factors, b_factors, degree):
    result = a_factors[0] / 2

    for k in range(1, degree + 1):
        result += a_factors[k] * cos(k * convert_x_to_v(x))
        result += b_factors[k] * sin(k * convert_x_to_v(x))

    return result


def calculate_a_factors(degree, n, points):
    result = []

    for k in range(0, degree + 1):
        a = 0
        for i in range(0, n):
            a += points[i][1] * cos(k * convert_x_to_v(points[i][0]))
        a *= 2 / n
        result.append(a)

    return result


def calculate_b_factors(degree, n, points):
    result = [0]

    for k in range(1, degree + 1):
        b = 0
        for i in range(0, n):
            b += points[i][1] * sin(k * convert_x_to_v(points[i][0]))
        b *= 2 / n
        result.append(b)

    return result


def trigonometric_approximation_for_domain(domain, n, points, degree):
    result = []
    difference = []

    a_factors = calculate_a_factors(degree, n, points)
    b_factors = calculate_b_factors(degree, n, points)

    for x in domain:
        result.append(trigonometric_approximation(x, a_factors, b_factors, degree))
        difference.append(f(x) - result[-1])

    print("norma euklidesowa roznicy funkcji aproksymowanej (" + str(degree) + " stopnia) i funkcji f: "
          + str(euclidean_norm(difference)))
    # print(str(degree) + " stopien: " + str(euclidean_norm(difference)))

    return result


def main():
    for n in range(5, 22):
        for degree in [3, 5, 7, 10]:
            if n == 21:
                n = 30

            a = -5
            b = 10

            points = []
            step = (b - a) / (n - 1)
            for i in range(n):
                x = a + i * step
                points.append((x, f(x)))

            domain = np.linspace(a, b, num=15000)

            print('\n\n' + str(n) + ' wezlow, stopien wielomianu aproksymujacego - ' + str(degree)
                  + ', wezly rownoodlegle')

            fig = plt.figure(1)
            fig.canvas.set_window_title(str(n) + ' wezlow, stopien wielomianu aproksymujacego - ' + str(degree))
            plt.subplots_adjust(hspace=0.5)

            plt.subplot(2, 1, 1)
            plt.grid(True)
            plt.plot(domain, trigonometric_approximation_for_domain(domain, n, points, degree), 'k-')
            plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
            plt.plot(domain, f_for_domain(domain), 'b--')
            plt.title('Wizualizacja funkcji aproksymujacej ' + str(degree) + ' stopnia')

            print(str(n) + ' wezlow, stopien wielomianu aproksymujacego - ' + str(degree)
                  + ', wezly Czebyszewa')

            points = []
            for i in range(1, n + 1):
                x = 0.5 * (a + b) + 0.5 * (b - a) * cos((2 * i - 1) * pi / (2 * n))
                points.append((x, f(x)))

            plt.subplot(2, 1, 2)
            plt.grid(True)
            plt.plot(domain, trigonometric_approximation_for_domain(domain, n, points, degree), 'k-')
            plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
            plt.plot(domain, f_for_domain(domain), 'b--')
            plt.title('Wizualizacja funkcji aproksymujacej ' + str(degree) + ' stopnia, wezly Czebyszewa')

            plt.show()


if __name__ == '__main__':
    main()
