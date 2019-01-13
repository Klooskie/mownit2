import numpy as np
import matplotlib.pyplot as plt
import sys
from math import sin, cos, pi, exp

# parametry zadania
m = 2


def maximum_norm(vector):
    maximum = 0
    for x in vector:
        if abs(x) > maximum:
            maximum = abs(x)
    return maximum


def solution(x):
    return cos(m * x) - x * sin(m * x)


def solution_for_domain(domain):
    result = []
    for x in domain:
        result.append(solution(x))
    return result


def finite_difference_method(x, points, n):
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


def finite_difference_method_for_domain(domain, n, x_0, x_k, a, b):
    result = []
    difference = []

    # obliczenie kroku
    step = (x_k - x_0) / n

    # wypelnienie listy kolejnych x
    x_col = []
    for i in range(n):
        x_col.append(x_0 + i * step)
    x_col.append(x_k)

    # stworzenie 3 kolumn macierzy trojdiagonalnej ukladu rownan i wektora wyrazow wolnych
    col_1 = []
    col_2 = []
    col_3 = []
    col_4 = []

    # wypelnienie macierzy trojdiagonalnej w zaleznosci od podanego rownania kolejno:
    # col_1 wspolczynnikami przy y_i-1
    # col_2 wspolczynnikami przy y_i
    # col_3 wspolczynnikami przy y_i+1
    # col_4 prawa strona rownania
    for i in range(1, n):
        col_1.append(1 / (step ** 2))
        col_2.append(((-2) / (step ** 2)) + m ** 2)
        col_3.append(1 / (step ** 2))
        col_4.append((-2) * m * cos(m * x_col[i]))

    # zmiana wartosci w pierwszej i ostatniej komorce wektora wyrazow wolnych z uwzglednieniem warunkow brzegowych
    col_4[0] -= col_1[0] * a
    col_4[n - 2] -= col_3[n - 2] * b

    # algorytm Thomasa
    # obliczenie wspolczynnikow B_0 i Y_0
    b_factors = [-col_3[0] / col_2[0]]
    y_factors = [col_4[0] / col_2[0]]

    # obliczenie kolejnych wspolczynnikow B i Y
    for i in range(1, n - 2):
        new_b_factor = -col_3[i]
        new_b_factor /= (col_1[i] * b_factors[-1] + col_2[i])

        new_y_factor = col_4[i] - col_1[i] * y_factors[-1]
        new_y_factor /= (col_1[i] * b_factors[-1] + col_2[i])

        b_factors.append(new_b_factor)
        y_factors.append(new_y_factor)

    # dodanie do wyniku y(x_k)
    y_col = [b]

    # obliczenie i dodanie do wyniku y(x_k-1)
    last_y = col_4[n - 2] - col_1[n - 2] * y_factors[-1]
    last_y /= (col_1[n - 2] * b_factors[-1] + col_2[n - 2])
    y_col.append(last_y)

    # obliczenie od konca kolejnych wartosci y(x)
    for i in range(n - 3, -1, -1):
        y_col.append(b_factors[i] * y_col[-1] + y_factors[i])

    # dodanie do wyniku y(x_0) i odwrocenie go
    y_col.append(a)
    y_col.reverse()

    # stworzenie listy obliczonych punktow nalezacych do wykresu y(x)
    points = []
    for i in range(n + 1):
        points.append((x_col[i], y_col[i]))

    # obliczenie wartosci y(x) dla kazdego z punktow na wykresie (y(x) jest aproksymowana splineami 1 stopnia)
    for point in domain:
        result.append(finite_difference_method(point, points, n))
        difference.append(solution(point) - result[-1])

    print("norma maksimum roznicy dokladnego rozwiazania i rozwiazania wyliczonego metoda roznic skonczonych\n"
          + "dla kroku rownego " + str(step) + " (podzial przedzialu na " + str(n) + " czesci):\n"
          + str(maximum_norm(difference)))
    return result


def main():
    # liczba krokow na przedziale [x_0, x_k]
    n = 5

    # parametry zadania, przedzial to [x_0, x_k], y(x_0) = a, y(x_k) = b
    x_0 = 0
    x_k = (2 * pi + 2) / m
    a = 1
    b = solution(x_k)

    print("\nRozpatrywany przedzial to: [" + str(x_0) + "; " + str(x_k) + "]\n")

    domain = np.linspace(x_0, x_k, num=1500)

    fig = plt.figure(1)
    fig.canvas.set_window_title('Rozwiazanie rownania rozniczkowego drugiego stopnia')
    plt.subplots_adjust(hspace=0.5)

    plt.subplot(111)
    plt.grid(True)
    plt.grid(True)
    plt.plot(domain, finite_difference_method_for_domain(domain, n, x_0, x_k, a, b), 'k-')
    plt.plot(domain, solution_for_domain(domain), 'b--')
    plt.title('y(x) uzyskane za pomoca metody roznic skonczonych')

    plt.show()


if __name__ == '__main__':
    main()
