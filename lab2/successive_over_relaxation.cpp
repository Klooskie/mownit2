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
        cout << endl;
    }
}

void print_augmented_matrix(int n, number ** A){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << "\t";
        }
        cout << "|\t" << A[i][n] << endl;
    }
}

void fill_vector_X(int n, number * X){
    // srand(time(NULL));
    for(int i = 0; i < n; i++){
        if(rand() % 2)
            X[i] = 1;
        else
            X[i] = -1;
    }
}

number * subtract_vectors(int n, number * X1, number * X2){
    number * X_diff = new number [n];
    for(int i = 0; i < n; i++){
        X_diff[i] = X1[i] - X2[i];
    }
    return X_diff;
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

void multiply(int n, number ** A, number * X, number * B){
    for(int i = 0; i < n; i++){
        B[i] = 0;
        for(int j = 0; j < n; j++){
            B[i] += A[i][j] * X[j];
        }
    }
}

void print_vector_and_norms(int n, number * X, number * X2){
    
    // cout << "\nwyliczony wektor niewiadomych" << endl;    
    // print_vector(n, X2);

    cout << "\nnorma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X2) <<endl;
    cout << "norma euklidesowa roznicy wektorow zadanego i obliczonego " << euclidean_norm(n, subtract_vectors(n, X, X2)) << endl;

    cout << "\nnorma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X2) <<endl;
    cout << "norma maksimum roznicy wektorow zadanego i obliczonego " << maximum_norm(n, subtract_vectors(n, X, X2)) << endl;

}

void fill_A_matrix(int n, number ** A){
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
}

bool stopping_rule_1(int n, number * X, number * newX, number ro){

    number * X_diff = new number[n];

    for(int i = 0; i < n; i++){
        X_diff[i] = newX[i] - X[i];
    }

    // cout << "roznica miedzy wektorem niewiadomych z kolejnej iteracji, a aktualnym ";
    // cout << maximum_norm(n, X_diff) << endl;

    if(maximum_norm(n, X_diff) < ro)
        return true;
    else
        return false;
}

bool stopping_rule_2(int n, number * X_new, number ** A, number ro){

    number * B = new number[n];
    number * B_diff = new number[n];

    multiply(n, A, X_new, B);    

    for(int i = 0; i < n; i++){
        B_diff[i] = B[i] - A[i][n];
    }

    // cout << "roznica miedzy wektorem prawej strony wyliczonym z aktualnego wektora niewiadomych, a zadanym ";
    // cout << maximum_norm(n, B_diff) << endl;

    if(maximum_norm(n, B_diff) < ro)
        return true;
    else
        return false;
}

number * SOR(int n, number ** A, number omega, number ro, int stopping_rule_number){

    //alokacja i wypelnienie poczatkowego wektora niewiadomych X
    number * X = new number[n];
    for(int i = 0; i < n; i++)
        X[i] = 0;

    //alokacja nowego wektora X i zmiennej przechowujacej liczbe iteracji
    number * X_new = new number[n];
    int number_of_iterations = 0;

    while(true){
        //sleep(1);

        for(int i = 0; i < n; i++){
            
            X_new[i] = (1 - omega) * X[i];
            X_new[i] += (omega / A[i][i]) * A[i][n];
            for(int j = 0; j < i; j++)
                X_new[i] -= (omega / A[i][i]) * A[i][j] * X_new[j];
            for(int j = i + 1; j < n; j++)
                X_new[i] -= (omega / A[i][i]) * A[i][j] * X[j];

        }

        // cout << "\nnajnowszy wektor niewiadomych" << endl;
        // print_vector(n, X_new);

        //sprawdzenie pierwszego warunku stopu
        if(stopping_rule_number == 1 && stopping_rule_1(n, X, X_new, ro))
            break;

        //sprawdzenie drugiego warunku stopu
        if(stopping_rule_number == 2 && stopping_rule_2(n, X, A, ro))
            break;

        number_of_iterations++;


        for(int i = 0; i < n; i++){
            X[i] = X_new[i];
        }    
    }
    
    cout << "\nliczba iteracji " << number_of_iterations << endl;

    return X;
}

int main(){
    int n, stopping_rule_number;
    number ro, omega;
    cout << "wpisz rozmiar macierzy, ro z warunku stopu, omege z przedzialu (1, 2) i numer warunku stopu do wykorzystania" << endl;
    cin >> n >> ro >> omega >> stopping_rule_number;
    while(stopping_rule_number != 1 && stopping_rule_number != 2){
        cout << "podaj numer warunku stopu - 1 lub 2" << endl;
        cin >> stopping_rule_number;
    }

    number ** A = new number * [n];
    for(int i = 0; i < n; i++)
        A[i] = new number[n+1];

    fill_A_matrix(n, A);

    //stworzenie i wypelnienie wektora niewiadomych
    number * X = new number[n];
    fill_vector_X(n, X);

    // cout << "\nwektor niewiadomych - x" <<endl;
    // print_vector(n, X);

    //wyliczenie wektora prawej strony
    number * B = new number[n];
    multiply(n, A, X, B);
    for(int i = 0; i < n; i++){
        A[i][n] = B[i];
    }

    // cout << "\nmacierz" << endl;
    // print_augmented_matrix(n, A);

    number * X2;
    X2 = SOR(n, A, omega, ro, stopping_rule_number);

    print_vector_and_norms(n, X, X2);

}