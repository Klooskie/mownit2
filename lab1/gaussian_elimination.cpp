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

void multiply(int n, number ** A, number * X, number * B){
    for(int i = 0; i < n; i++){
        B[i] = 0;
        for(int j = 0; j < n; j++){
            B[i] += A[i][j] * X[j];
        }
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

void print_vector_and_norms(int n, number * X, number * X2){
    
    cout << "\nwyliczony wektor niewiadomych" << endl;
    print_vector(n, X2);

    cout << "\nnorma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X2) <<endl;
    cout << "roznica norm euklidesowych " << absolute(euclidean_norm(n, X) - euclidean_norm(n, X2)) << endl;

    cout << "\nnorma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X2) <<endl;
    cout << "roznica norm maksimum " << absolute(maximum_norm(n, X) - maximum_norm(n, X2)) << endl;

}

void experiment(int n, number ** A){
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

    // cout << "\nwektor prawej strony - b" << endl;
    // print_vector(n, B);

    // cout << "\nmacierz" << endl;
    // print_matrix(n, A);

    gaussian_elimination(n, A);

    // cout << "\nmacierz po eliminacji" << endl;
    // print_matrix(n, A);

    number * X2 = new number[n];
    calculate_variables(n, A, X2);
    
    print_vector_and_norms(n, X, X2);

}

void zad1(int n){

    //alokacja macierzy
    number ** A = new number * [n];
    for(int i = 0; i < n; i++)
        A[i] = new number[n + 1];

    //wypelnienie pierwszego wiersza
    for(int i = 0; i < n; i++){
        A[0][i] = 1;
    }
    
    //wypelnienie reszty macierzy
    for(int i = 1; i < n; i++){
        for(int j = 0; j < n; j++){
            A[i][j] = ((number) 1) / ((number) i + (number) j + 1); //+1 bo indeksy od 0
        }
    }

    experiment(n, A);
}

void zad2(int n){

    //alokacja macierzy
    number ** A = new number * [n];
    for(int i = 0; i < n; i++)
        A[i] = new number[n + 1];

    //wypelnienie macierzy
    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            A[i][j] = 2 * ((number) (i + 1)) / ((number) (j + 1));
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < i; j++){
            A[i][j] = A[j][i];
        }
    }

    experiment(n, A);
}

void print_time_and_norms(struct timeval time_start, struct timeval time_end, int n, number * X, number * X2){

    cout << "Czas trwania: " << (double)(time_end.tv_sec - time_start.tv_sec) + ((double)time_end.tv_usec - (double)time_start.tv_usec) / 1000000 << endl << endl;

    cout << "norma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X2) <<endl;
    cout << "roznica norm euklidesowych " << absolute(euclidean_norm(n, X) - euclidean_norm(n, X2)) << endl << endl;

    cout << "norma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X2) <<endl;
    cout << "roznica norm maksimum " << absolute(maximum_norm(n, X) - maximum_norm(n, X2)) << endl << endl;

}

void zad3_gauss(int n, number * X){

    //alokacja macierzy i wypelnienie jej zerami
    number ** A = new number * [n];
    for(int i = 0; i < n; i++){
        A[i] = new number[n + 1];
        for(int j = 0; j < n + 1; j++){   
            A[i][j] = 0;
        }
    }

    //wypelnienie macierzy
    A[0][0] = -6; //-3 * 1 - 3
    A[0][1] = 1; 
    for(int i = 1; i < n-1; i++){
        
        A[i][i-1] = 3 / ((number) (i + 1));
        A[i][i] = (-3) * ((number) (i + 1)) - 3;
        A[i][i+1] = (number) i + 1;

    }
    A[n-1][n-2] = 3 / ((number) n);
    A[n-1][n-1] = (-3) * ((number) n) - 3;

    //wyliczenie wektora prawej strony
    number * B = new number[n];
    multiply(n, A, X, B);
    for(int i = 0; i < n; i++){
        A[i][n] = B[i];
    }

    // wypisanie macierzy
    // print_matrix(n, A);

    //zapamietanie czasu poczatkowego
    struct timeval time_start, time_end;
    gettimeofday(&time_start, NULL);

    //eliminacja gaussa
    gaussian_elimination(n, A);
    
    //wyliczenie wektora niewiadomych
    number * X2 = new number[n];
    calculate_variables(n, A, X2);

    //zapamietanie czasu koncowego
    gettimeofday(&time_end, NULL);

    cout << "\nEliminacja Gaussa:" << endl;
    print_time_and_norms(time_start, time_end, n, X, X2);

}

void zad3_thomas(int n, number * X){
    
    //alokacja macierzy
    number ** A = new number * [n];
    for(int i = 0; i < n; i++){
        A[i] = new number[4];
    }

    //wypelnienie macierzy
    A[0][0] = 0;
    A[0][1] = -6;
    A[0][2] = 1;
    for(int i = 1; i < n - 1; i++){
        A[i][0] = 3 / ((number) (i + 1));
        A[i][1] = (-3) * ((number) (i + 1)) - 3;
        A[i][2] = (number) i + 1;
    } 
    A[n-1][0] = 3 / ((number) n);
    A[n-1][1] = (-3) * ((number) n) - 3;
    A[n-1][2] = 0;

    //wyliczenie wektora prawej strony
    for(int i = 1; i < n - 1; i++){
        A[i][3] = 0;
        for(int j = 0; j < 3; j++){
            A[i][3] += A[i][j] * X[i + j - 1];
        }
    }
    A[0][3] = A[0][1] * X[0] + A[0][2] * X[1];
    A[n-1][3] = A[n-1][0] * X[n-2] + A[n-1][1] * X[n-1];

    // wypisanie macierzy
    // for(int i = 0; i < n; i++){
    //     cout << A[i][0] << "\t" << A[i][1] << "\t" << A[i][2] << "\t->\t" << A[i][3] << endl;
    // }

    //zapamietanie czasu poczatkowego
    struct timeval time_start, time_end;
    gettimeofday(&time_start, NULL);

    //Wyliczenie wartosci Beta 0
    number * B = new number[n]; 
    B[0] = (- A[0][2]) / A[0][1];

    //Wyliczenie wartosci Gamma 0
    number * Y = new number[n];
    Y[0] = A[0][3] / A[0][1];

    //Wyliczenie kolejnych wartosci Beta i Gamma
    for(int i = 1; i < n; i++){
        B[i] = (- A[i][2])/(A[i][0] * B[i-1] + A[i][1]);
        Y[i] = (A[i][3] - (A[i][0] * Y[i-1]))/(A[i][0] * B[i-1] + A[i][1]);
    } 

    //wyliczenie wektora niewiadomych
    number * X2 = new number[n];
    X2[n-1] = Y[n-1];

    for(int i = n - 2; i >= 0; i--){
        X2[i] = B[i] * X2[i+1] + Y[i];
    }

    gettimeofday(&time_end, NULL);

    cout << "\n\nAlgorytm Thomasa:" << endl;
    print_time_and_norms(time_start, time_end, n, X, X2);
}

void zad3(int n){

    //utworzenie i wypelnienie wektora niewiadomych
    number * X = new number[n];
    fill_vector_X(n, X);
    
    zad3_gauss(n, X);
    zad3_thomas(n, X);

}

int main(){

    int n;
    cout << "wpisz rozmiar macierzy" << endl;
    cin >> n;

    // zad1(n);
    // zad2(n);
    zad3(n);

}
