#include <iostream>
#include <cmath>
using namespace std;
typedef double number;

//  h := 1 /* Initialization of the pivot row */
//  k := 1 /* Initialization of the pivot column */
//  while h ≤ m and k ≤ n
//    /* Find the k-th pivot: */
//    i_max := argmax (i = h ... m, abs(A[i, k]))
//    if A[i_max, k] = 0
//      /* No pivot in this column, pass to next column */
//      k := k+1
//    else
//       swap rows(h, i_max)
//       /* Do for all rows below pivot: */
//       for i = h + 1 ... m:
//          f := A[i, k] / A[h, k]
//          /* Fill with zeros the lower part of pivot column: */
//          A[i, k]  := 0
//          /* Do for all remaining elements in current row: */
//          for j = k + 1 ... n:
//             A[i, j] := A[i, j] - A[h, j] * f
//       /* Increase pivot row and column */
//       h := h+1 
//       k := k+1

number absolute(number x){
    return x < 0 ? (-1)*x : x;
}

void swap_rows(int n, number ** A, int row1, int row2){
    number Tmp[n+1];

    for(int i = 0; i <= n; i++){
        Tmp[i] = A[row1][i];
        A[row1][i] = A[row2][i];
        A[row2][i] = Tmp[i];
    }
}

void elimination(int n, number ** A){

    int row = 0; //nr rzedu pivotu
    int column = 0; //nr kolumny pivtu
    while(row < n and column <= n){

        //znajdywanie najwiekszej wartosci w ktej kolumnie w pozostalych wierszach
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

void count_variables(int n, number ** A, number * X){
    //wyliczanie niewiadomych
    for(int i = n - 1; i >= 0; i--){

        int s = A[i][n];
        for(int j = n - 1; j > i; j--){ //popraw warunek konca
            s -= A[i][j] * X[j];
        }
        if(A[i][i] == 0){
            cout<<"\n\nZero na przekątnej - uklad nie ma dokladnie 1 rozwiazania"<<endl;
            return;
        }
        X[i] = s / A[i][i];
    }

    //wypsiz
    cout<<"\n\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << " ";
        }
        cout << "\t|\t" << X[i] << "\t|\t" << A[i][n] << endl;
    }
}

void multiply(int n, number ** A, number * X, number * B){

    for(int i = 0; i < n; i++){

        B[i] = 0;

        for(int j = i; j < n; j++){

            B[i] += A[i][j] * X[j];

        }
    }

}

void count_differences(int n, number ** A, number * B, number * Diff){

    for(int i = 0; i < n; i++){
        Diff[i] = absolute(B[i] - A[i][n]);
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

int main(){

    number **A; //rozszerzona macierz ukladu
    int n;

    cout << "wpisz liczbe niewiadomych" << endl;

    cin >> n;

    A = new number*[n];
    for(int i = 0; i < n; i++)
        A[i] = new number[n + 1];

    printf("wpisz kolejno wiersze tablicy\n");

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            cin >> A[i][j];

    printf("wpisz wartosci wektora prawej strony\n");

    for(int i = 0; i < n; i++)
        cin >> A[i][n];

    //wypisz
    cout<<"\n\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << " ";
        }
        cout << "\t|\tx" << i << "\t|\t" << A[i][n] << endl;
    }


    elimination(n, A);

    number * X;
    X = new number[n];

    count_variables(n, A, X);

    number * B = new number[n];
    multiply(n, A, X, B);

    cout<<"\n\n";
    for(int i = 0; i < n; i++){
        cout<<B[i]<<endl;
    }

    number * Diff = new number[n];
    count_differences(n, A, B, Diff);

    cout<<"\n\n";
    for(int i = 0; i < n; i++){
        cout<<Diff[i]<<endl;
    }


    number enorm = euclidean_norm(n, X);
    number mnorm = maximum_norm(n, X);

    cout<<"\n\n"<<enorm<<endl<<mnorm<<endl;
}


