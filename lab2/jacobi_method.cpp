#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
typedef float number;


number absolute(number x){
    return x < 0 ? (-1) * x : x;
}

void print_vector(int n, number * A){
    for(int i = 0; i < n; i++){
        cout << A[i] << endl;
    }
}

void print_matrix(int n, number ** A){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << "\t";
        }
        cout << "|\t" << A[i][n] << endl;
    }
}

void fill_vector_X(int n, number * X){
    srand(time(NULL));
    for(int i = 0; i < n; i++){
        if(rand() % 2)
            X[i] = 1;
        else
            X[i] = -1;
    }
}

void swap_rows(int n, number ** A, int row1, int row2){
    number Tmp[n+1];

    for(int i = 0; i <= n; i++){
        Tmp[i] = A[row1][i];
        A[row1][i] = A[row2][i];
        A[row2][i] = Tmp[i];
    }
}

number euclidean_norm(int n, number * X){
    number sum_of_squares = 0;
    for(int i = 0; i < n; i++){
        sum_of_squares += X[i] * X[i];
    }
    return sqrt(sum_of_squares);
}

number maximum_norm(int n, number * X){
    number max_number = 0;
    for(int i = 0; i < n; i++){
        number tmp = absolute(X[i]);

        if(tmp > max_number)
            max_number = tmp;
    }
    return max_number;
}

number * jacobi_method(int n, number ** A, int number_of_iterations){
    //alokacja i wypelnienie poczatkowego wektora niewiadomych X
    number * X = new number[n];
    for(int i = 0; i < n; i++)
        X[i] = 0;

    //odwrocenie wartosci na przekatnej macierzy - traktujemy wtedy te przekatna jako N = D ^ (-1)
    for(int i = 0; i < n; i++){
        A[i][i] = 1 / A[i][i];
    }

    //obliczenie wartości N*b, gdzie b to wektor prawej strony
    number * Nb = new number[n];
    for(int i = 0; i < n; i++){
        Nb[i] = A[i][i] * A[i][n];
    }

    //obliczenie macierzy M = -N * (L + U)
    number ** M = new number * [n];
    for(int i = 0; i < n; i++){
        M[i] = new number[n];

        for(int j = 0; j < n; j++){

            M[i][j] = 0;
            if(i != j) 
                M[i][j] -= A[i][i] * A[i][j];
        }
    }

    //wykonanie number_of_iterations iteracji przyblizania wektora X
    for(int iteration = 0; iteration < number_of_iterations; iteration++){

        number * newX = new number[n];
        for(int i = 0; i < n; i++){
            newX[i] = Nb[i];
            for(int j = 0; j < n; j++){
                newX[i] += (M[i][j] * X[j]);
            }
        }

        for(int i = 0; i < n; i++){
            X[i] = newX[i];
        }

        // cout<<endl;
        // print_vector(n, X);        
    }

    return X;
}



//eksperyment jak na 1 labach

void multiply(int n, number ** A, number * X, number * B){
    for(int i = 0; i < n; i++){
        B[i] = 0;
        for(int j = 0; j < n; j++){
            B[i] += A[i][j] * X[j];
        }
    }
}

void print_vector_and_norms(int n, number * X, number * X2){
    
    print_vector(n, X2);

    cout << "\nnorma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X2) <<endl;
    cout << "roznica norm euklidesowych " << absolute(euclidean_norm(n, X) - euclidean_norm(n, X2)) << endl;

    cout << "\nnorma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X2) <<endl;
    cout << "roznica norm maksimum " << absolute(maximum_norm(n, X) - maximum_norm(n, X2)) << endl;

}

void experiment(int n, number ** A, int number_of_iterations){
    //stworzenie i wypelnienie wektora niewiadomych
    number * X = new number[n];
    fill_vector_X(n, X);

    cout << "\nwektor niewiadomych - x" <<endl;
    print_vector(n, X);

    //wyliczenie wektora prawej strony
    number * B = new number[n];
    multiply(n, A, X, B);
    for(int i = 0; i < n; i++){
        A[i][n] = B[i];
    }

    cout << "\nwektor prawej strony - b" << endl;
    print_vector(n, B);

    cout << "\nmacierz" << endl;
    print_matrix(n, A);

    number * X2 = new number[n];
    X2 = jacobi_method(n, A, number_of_iterations);

    cout << "\nwyliczony wektor niewiadomych" << endl;    
    print_vector_and_norms(n, X, X2);

}

//koniec eksperymentu

/*
void gaussian_elimination(int n, number ** A){

    int row = 0; //nr rzedu pivotu
    int column = 0; //nr kolumny pivtu
    while(row < n and column <= n){

        //znajdywanie najwiekszej wartosci w kolumnie pivocie w pozostalych wierszach
        number max_value = 0;
        int max_value_row = row;

        for(int i = row; i < n; i++){

            number tmp = absolute(A[i][column]);

            if(max_value < tmp){
                max_value = tmp;
                max_value_row = i;
            }
        }

        if(max_value == 0){
            //wszystkie wiersze w tej kolumnie maja wartosc 0
            column++;
        }
        else{
            //jesli w wierszu pivocie mamy 0 w kolumnie pivocie to podmieniamy rzad
            if(A[row][column] == 0)
                swap_rows(n, A, row, max_value_row);

            //dla pozostalych wierszy odejmujemy odpowiedno przemnozony rzad pivot
            for(int i = row + 1; i < n; i++){
                number factor = A[i][column] / A[row][column];

                A[i][column] = 0;
                for(int j = column + 1; j <= n; j++){
                    A[i][j] = A[i][j] - factor * A[row][j];
                }
            }
         
            row++;
            column++;
        }
    }
}


void calculate_variables(int n, number ** A, number * X){
    for(int i = n - 1; i >= 0; i--){

        number s = A[i][n];
        for(int j = n - 1; j > i; j--){
            s -= A[i][j] * X[j];
        }
        if(A[i][i] == 0){
            cout << "\nZero na przekątnej - uklad nie ma dokladnie 1 rozwiazania" << endl;
            exit(0);
        }
        X[i] = s / A[i][i];
    }
}

*/

void zad1(int n, int number_of_iterations){
    
    number ** A = new number * [n];
    for(int i = 0; i < n; i++){
        A[i] = new number[n+1];
    }

    cout << "wpisz macierz rozszerzona" << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n + 1; j++){
            cin >> A[i][j];
        }
    }

    experiment(n, A, number_of_iterations);
}

int main(){
    int n, number_of_iterations;
    cout << "wpisz rozmiar macierzy i liczbe iteracji" << endl;
    cin >> n >> number_of_iterations;

    zad1(n, number_of_iterations);

}