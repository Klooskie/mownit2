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


def calculate_factors_cubic(n, points):
    h = []
    b = []
    u = []
    v = []
    z = []
    for i in range(n - 1):
        h.append(points[i + 1][0] - points[i][0])
        b.append((points[i + 1][1] - points[i][1]) / h[i])

    u.append(0)
    v.append(0)
    u.append(2 * (h[0] + h[1]))
    v.append(6 * (b[1] - b[0]))

    for i in range(2, n - 1):
        u_value = 2 * (h[i] - h[i - 1])
        u_value -= h[i - 1] * h[i - 1] / u[i - 1]
        u.append(u_value)

        v_value = 6 * (b[i] - b[i - 1])
        v_value -= h[i - 1] * v[i - 1] / u[i - 1]
        v.append(v_value)

    # miejsce na warunki brzegowe !!!
    z.append(0)
    for i in range(n - 2, 0, -1):
        z_value = v[i] - h[i] * z[-1]
        z_value /= u[i]
        z.append(z_value)
    z.append(0)
    z.reverse()

    return z


def cubic_spline(x, n, points, factors):
    i = 0
    while x > points[i + 1][0]:
        if i + 1 < n - 1:
            i += 1
        else:
            break

    h = points[i+1][0] - points[i][0]

    result = (x - points[i][0]) * (factors[i+1] - factors[i])
    result /= (6 * h)
    result += factors[i] / 2

    result *= (x - points[i][0])
    result -= (h / 6) * (factors[i+1] + 2 * factors[i])
    result += (points[i+1][1] - points[i][1]) / h

    result *= (x - points[i][0])
    result += points[i][1]

    return result


def cubic_spline_for_domain(domain, n, points, boundary_condition):
    result = []
    difference = []

    factors = calculate_factors_cubic(n, points)
    for x in domain:
        result.append(cubic_spline(x, n, points, factors))
        difference.append(f(x) - result[-1])

    print("norma maksimum roznicy funkcji sklejanej (drugiego stopnia) i funkcji f: " + str(maximum_norm(difference)))
    return result


def choose_z0(boundary_condition, points):
    if boundary_condition == 0:
        # pierwszy warunek brzegowy Q'(t0) = 0
        return 0
    else:
        # drugi warunek brzegowy Q''(t0) = 0
        return (points[1][1] - points[0][1]) / (points[1][0] - points[0][0])


def calculate_factors_quadratic(n, points, z0):
    factors = [z0]
    for i in range(n - 1):
        new_factor = 2 * (points[i + 1][1] - points[i][1])
        new_factor /= (points[i + 1][0] - points[i][0])
        new_factor -= factors[-1]
        factors.append(new_factor)
    return factors


def quadratic_spline(x, n, points, factors):
    i = 0
    while x > points[i + 1][0]:
        if i + 1 < n - 1:
            i += 1
        else:
            break

    result = factors[i + 1] - factors[i]
    result /= 2 * (points[i + 1][0] - points[i][0])
    result *= (x - points[i][0]) ** 2
    result += factors[i] * (x - points[i][0])
    result += points[i][1]

    return result


def quadratic_spline_for_domain(domain, n, points, boundary_condition):
    result = []
    difference = []

    z0 = choose_z0(boundary_condition, points)
    factors = calculate_factors_quadratic(n, points, z0)

    for x in domain:
        result.append(quadratic_spline(x, n, points, factors))
        difference.append(f(x) - result[-1])

    print("norma maksimum roznicy funkcji sklejanej (drugiego stopnia) i funkcji f: " + str(maximum_norm(difference)))
    return result


def f(x):
    # return sin(4 * x / pi) * exp(-0.2 * x / pi)
    return x ** 3


def f_for_domain(domain):
    result = []
    for x in domain:
        result.append(f(x))
    return result


def main():
    for n in range(3, 21):

        a = -5
        b = 10
        boundary_condition_quadratic = 1
        boundary_condition_cubic = 1

        points = []
        step = (b - a) / (n - 1)
        for i in range(n):
            x = a + i * step
            points.append((x, f(x)))

        domain = np.linspace(a-1, b+1, num=1500)

        print('\n\nWezly (' + str(n) + ') rozmieszczone rownomiernie na calym przedziale')

        fig = plt.figure(1)
        fig.canvas.set_window_title('Wezly (' + str(n) + ') rozmieszczone rownomiernie na calym przedziale')
        plt.subplots_adjust(hspace=0.5)

        plt.subplot(211)
        plt.plot(domain, quadratic_spline_for_domain(domain, n, points, boundary_condition_quadratic), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolacji funkcja sklejana drugiego stopnia\'a')

        plt.subplot(212)
        plt.plot(domain, cubic_spline_for_domain(domain, n, points, boundary_condition_cubic), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Wizualizacja interpolacji funkcja sklejana trzeciego stopnia\'a')

        # points = []
        # for i in range(1, n + 1):
        #     x = 0.5 * (a + b) + 0.5 * (b - a) * cos((2 * i - 1) * pi / (2 * n))
        #     points.append((x, f(x)))
        #
        # print('\n\nWezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czebyszewa')
        #
        # fig = plt.figure(2)
        # fig.canvas.set_window_title('Wezly (' + str(n) + ') rozmieszczone zgodnie z zerami wielomianu Czebyszewa')
        # plt.subplots_adjust(hspace=0.5)
        #
        # plt.subplot(211)
        # plt.plot(domain, quadratic_spline_for_domain(domain, n, points, boundary_condition_quadratic), 'k-')
        # plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        # plt.plot(domain, f_for_domain(domain), 'b--')
        # plt.title('Wizualizacja interpolacji funkcja sklejana drugiego stopnia\'a')
        #
        # plt.subplot(212)
        # plt.plot(domain, cubic_spline_for_domain(domain, n, points, boundary_condition_cubic), 'k-')
        # plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        # plt.plot(domain, f_for_domain(domain), 'b--')
        # plt.title('Wizualizacja interpolacji funkcja sklejana trzeciego stopnia\'a')

        plt.show()


if __name__ == '__main__':
    main()
