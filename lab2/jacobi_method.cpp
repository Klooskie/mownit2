#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using namespace std;
typedef double number;


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

void multiply(int n, number ** A, number * X, number * B){
    for(int i = 0; i < n; i++){
        B[i] = 0;
        for(int j = 0; j < n; j++){
            B[i] += A[i][j] * X[j];
        }
    }
}

bool warunek_stopu_1(int n, number * X, number * newX, number ro){

    number * diffX = new number[n];

    for(int i = 0; i < n; i++){
        diffX[i] = newX[i] - X[i];
    }

    if(maximum_norm(n, diffX) < ro)
        return true;
    else
        return false;
}

bool warunek_stopu_2(int n, number * newX, number ** A, number ro){

    number * B = new number[n];
    number * diffB = new number[n];

    multiply(n, A, newX, B);    

    for(int i = 0; i < n; i++){
        diffB[i] = B[i] - A[i][n];
    }

    cout<<"wyliczony wektor"<<endl;
    print_vector(n, B);
    cout << "roznica " << maximum_norm(n, diffB) << endl;
sleep(1);
    if(maximum_norm(n, diffB) < ro)
        return true;
    else
        return false;
}

number * jacobi_method(int n, number ** A, number ro){
    //alokacja i wypelnienie poczatkowego wektora niewiadomych X
    number * X = new number[n];
    for(int i = 0; i < n; i++)
        X[i] = 0;

    number ** copyA = new number * [n];
    for(int i = 0; i < n; i++){
        copyA[i] = new number [n+1];
        for(int j = 0; j < n+1; j++){
            copyA[i][j] = A[i][j];
        }
    }

    //odwrocenie wartosci na przekatnej macierzy - traktujemy wtedy ta przekatna jako N = D ^ (-1)
    for(int i = 0; i < n; i++){
        A[i][i] = 1 / A[i][i];
    }

    //obliczenie wartości N*b, gdzie b to wektor prawej strony
    number * Nb = new number[n];
    for(int i = 0; i < n; i++){
        Nb[i] = A[i][i] * A[i][n];
    }

    //obliczenie macierzy iteracji M = -N * (L + U)
    number ** M = new number * [n];
    for(int i = 0; i < n; i++){
        M[i] = new number[n];

        for(int j = 0; j < n; j++){

            M[i][j] = 0;
            if(i != j) 
                M[i][j] -= A[i][i] * A[i][j];
        }
    }

    // cout<<"\nmacierz iteracji"<<endl;
    // print_matrix(n, M);
    
    //alokacja nowego wektora X i zmiennej przechowujacej liczbe iteracji
    number * newX = new number[n];
    int number_of_iterations = 0;

    //wykonywanie iteracji dopoki nie zajdzie warunek stopu
    while(true){
        number_of_iterations++;

        //obliczenie nowego wektora X
        for(int i = 0; i < n; i++){
            newX[i] = Nb[i];
            for(int j = 0; j < n; j++){
                newX[i] += (M[i][j] * X[j]);
            }
        }

        // if(warunek_stopu_1(n, X, newX, ro))
        //     break;

        if(warunek_stopu_2(n, newX, copyA, ro))
            break;

        for(int i = 0; i < n; i++){
            X[i] = newX[i];
        }    
    }

    cout << "\nliczba iteracji " << number_of_iterations << endl;

    return newX;
}

void experiment(int n, number ** A, number ro){
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

    cout << "\nmacierz" << endl;
    print_matrix(n, A);

    number * X2 = new number[n];
    X2 = jacobi_method(n, A, ro);

    print_vector_and_norms(n, X, X2);

}

void zad1(int n, number ro){

    //alokacja macierzy A    
    number ** A = new number * [n];
    for(int i = 0; i < n; i++){
        A[i] = new number[n+1];
    }

    //wypelnienie macierzy A
    for(int i = 0; i < n; i++){
        //wypelnienie prezkatnej
        A[i][i] = 5;
    
        //wypelnienie pol po lewej od przekatnej
        if(i != 0){
            A[i][i-1] = 0.25 / ((number) i + 1);
        }

        //wypelnienie pol nad przekatna
        for(int j = i + 1; j < n; j++){
            if((j + 1) % 2 == 0)
                A[i][j] = 0.25 / ((number) j + 1);
            else
                A[i][j] = (-1) * 0.25 / ((number) j + 1);
        }

        //wypelnienie pozostalych pol
        if(i >= 2){
            for(int j = i - 2; j >= 0; j--){
                A[i][j] = 0;
            }
        }
    }

    experiment(n, A, ro);
}

int main(){
    int n;
    number ro;
    cout << "wpisz rozmiar macierzy i ro z warunku stopu" << endl;
    cin >> n >> ro;

    zad1(n, ro);

}