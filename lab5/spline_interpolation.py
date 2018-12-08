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


def calculate_x(points):
    result = f_derivative(points[0][0])
    result -= ((points[1][1] - points[0][1]) / (points[1][0] - points[0][0]))
    result *= 6
    result /= (points[1][0] - points[0][0])
    return result


def calculate_y(points, n):
    result = f_derivative(points[n - 1][0])
    result -= ((points[n - 1][1] - points[n - 2][1]) / (points[n - 1][0] - points[n - 2][0]))
    result *= 6
    result /= (points[n - 1][0] - points[n - 2][0])
    return result


def calculate_factors_cubic(n, points, boundary_condition):
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

    for i in range(1, n - 1):
        u_value = 2 * (h[i] + h[i - 1])
        u.append(u_value)

        v_value = 6 * (b[i] - b[i - 1])
        v.append(v_value)

    if boundary_condition == 1:
        # pierwszy warunek brzegowy S"(t_0) = 0 and S"(t_n-1) = 0

        # metoda Thomasa dla naszej macierzy
        betas = []
        gammas = []
        betas.append(0)
        gammas.append(v[0])

        for i in range(1, n - 1):
            gamma_value = v[i] - (h[i - 1] * gammas[-1])
            gamma_value /= (h[i - 1] * betas[-1] + u[i])
            gammas.append(gamma_value)

            beta_value = (-1) * h[i]
            beta_value /= (h[i - 1] * betas[-1] + u[i])
            betas.append(beta_value)

        z.append(0)
        for i in range(n - 2, 0, -1):
            z.append(betas[i] * z[-1] + gammas[i])
        z.append(0)

    else:
        # drugi warunek brzegowy S'(t_0) = f'(t_0) and S'(t_n-1) = f'(t_n-1)

        x = calculate_x(points)
        y = calculate_y(points, n)

        # metoda Thomasa dla naszej macierzy
        betas = []
        gammas = []
        betas.append((-1) * h[1] / (u[1] - (h[0] / 2)))
        gammas.append((v[1] + (h[0] * x / 2)) / (u[1] - (h[0] / 2)))

        for i in range(1, n - 3):
            gamma_value = v[i + 1] - (h[i] * gammas[-1])
            gamma_value /= (h[i] * betas[-1] + u[i + 1])
            gammas.append(gamma_value)

            beta_value = (-1) * h[i + 1]
            beta_value /= (h[i] * betas[-1] + u[i + 1])
            betas.append(beta_value)

        z_second_to_last = (v[n - 2] - (h[n - 2] * y / 2)) - (h[n - 3] * gammas[-1])
        z_second_to_last /= (h[n - 3] * betas[-1] + u[n - 2])

        z.append((y - z_second_to_last) / 2)
        z.append(z_second_to_last)
        for i in range(n - 4, -1, -1):
            z.append(betas[i] * z[-1] + gammas[i])
        z.append((x + z[-1]) / (-2))

    z.reverse()
    return z


def cubic_spline(x, n, points, factors):
    i = 0
    while x > points[i + 1][0]:
        if i + 1 < n - 1:
            i += 1
        else:
            break

    h = points[i + 1][0] - points[i][0]
    result = ((x - points[i][0]) ** 3) * factors[i + 1] / (6 * h)
    result += ((points[i + 1][0] - x) ** 3) * factors[i] / (6 * h)
    result += (points[i + 1][1] / h - factors[i + 1] * h / 6) * (x - points[i][0])
    result += (points[i][1] / h - h * factors[i] / 6) * (points[i + 1][0] - x)

    return result


def cubic_spline_for_domain(domain, n, points, boundary_condition):
    result = []
    difference = []

    factors = calculate_factors_cubic(n, points, boundary_condition)
    for x in domain:
        result.append(cubic_spline(x, n, points, factors))
        difference.append(f(x) - result[-1])

    print("norma maksimum roznicy funkcji f i funkcji sklejanej (trzeciego stopnia), przy warunku brzegowym "
          + str(boundary_condition) + ": " + str(maximum_norm(difference)))
    return result


def choose_z0(boundary_condition, points):
    if boundary_condition == 1:
        # pierwszy warunek brzegowy Q'(t0) = 0
        return 0
    else:
        # drugi warunek brzegowy Q"(t0) = 0
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

    print("norma maksimum roznicy funkcji f i funkcji sklejanej (drugiego stopnia), przy warunku brzegowym "
          + str(boundary_condition) + ": " + str(maximum_norm(difference)))
    return result


def main():
    for n in range(4, 22):
        if n == 21:
            n = 30

        a = -5
        b = 10

        points = []
        step = (b - a) / (n - 1)
        for i in range(n):
            x = a + i * step
            points.append((x, f(x)))

        domain = np.linspace(a, b, num=1900)

        # warunki brzegowe dla splajnow kwadratowych
        # 1) Q'(t0) = 0
        # 2) Q"0 = 0
        print('\n\n' + str(n) + ' wezlow, quadratic spline')

        fig = plt.figure(1)
        fig.canvas.set_window_title(str(n) + ' wezlow, quadratic spline')
        plt.subplots_adjust(hspace=0.5)

        plt.subplot(211)
        plt.grid(True)
        plt.plot(domain, quadratic_spline_for_domain(domain, n, points, 1), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Funkcja sklejana drugiego stopnia, warunek brzegowy 1')

        plt.subplot(212)
        plt.grid(True)
        plt.plot(domain, quadratic_spline_for_domain(domain, n, points, 2), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Funkcja sklejana drugiego stopnia, warunek brzegowy 2')

        # warunki brzegowe dla splajnow kubicznych
        # 1) S"(t_0) = 0, S"(t_n-1) = 0
        # 2) S'(t_0) = f'(t_0), S'(t_n-1) = f'(t_n-1)
        print('\n\n' + str(n) + ' wezlow, cubic spline')

        fig = plt.figure(2)
        fig.canvas.set_window_title(str(n) + ' wezlow, cubic spline')
        plt.subplots_adjust(hspace=0.5)

        plt.subplot(211)
        plt.grid(True)
        plt.plot(domain, cubic_spline_for_domain(domain, n, points, 1), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Funkcja sklejana trzeciego stopnia, warunek brzegowy 1')

        plt.subplot(212)
        plt.grid(True)
        plt.plot(domain, cubic_spline_for_domain(domain, n, points, 2), 'k-')
        plt.plot([point[0] for point in points], [point[1] for point in points], 'ro')
        plt.plot(domain, f_for_domain(domain), 'b--')
        plt.title('Funkcja sklejana trzeciego stopnia, warunek brzegowy 2')

        plt.show()


if __name__ == '__main__':
    main()
