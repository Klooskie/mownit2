#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
typedef double number;

void print_vector(int n, number * A){
    for(int i = 0; i < n; i++){
        cout << A[i] << endl;
    }
}

void print_matrix(int n, number ** A){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << " ";
        }
        cout << " | " << A[i][n] << endl;
    }
}

number absolute(number x){
    return x < 0 ? (-1) * x : x;
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
    //wyliczanie niewiadomych
    for(int i = n - 1; i >= 0; i--){

        int s = A[i][n];
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

void old_labs(){

    int n;
    cout << "wpisz rozmiar macierzy" << endl;
    cin >> n;

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
            A[i][j] = 1 / ((number) i + (number) j + 1); //+1 bo indeksy od 0
        }
    }

    number * X = new number[n];
    srand(time(NULL));
    for(int i = 0; i < n; i++){
        if(rand() % 2)
            X[i] = 1;
        else
            X[i] = -1;
    }

    cout << "\nwektor niewiadomych" <<endl;
    print_vector(n, X);

    number * B = new number[n];
    multiply(n, A, X, B);

    cout << "\nwektor prawej strony" << endl;
    print_vector(n, B);

    for(int i = 0; i < n; i++){
        A[i][n] = B[i];
    }

    cout << "\nmacierz" << endl;
    print_matrix(n, A);

    gaussian_elimination(n, A);
    cout << "\nmacierz po eliminacji" << endl;
    print_matrix(n, A);

    number * X2 = new number[n];
    calculate_variables(n, A, X2);

    cout << "\nwyliczony wektor niewiadomych" << endl;
    print_vector(n, X2);

    
    // B = new number[n];
    // multiply(n, A, X2, B);
    // cout<<"\nwektor prawej strony na podstawie nowego wektora niewiadomych"<<endl;
    // print_vector(n, B);

 
    cout << "\nnorma euk X " << euclidean_norm(n, X) << "\nnorma euk X2 " << euclidean_norm(n, X2) <<endl;
}

int main(){

    // old_labs(); exit(0);

    number **A; //rozszerzona macierz ukladu
    int n;

    cout << "wpisz liczbe niewiadomych" << endl;
    cin >> n;

    A = new number * [n];
    for(int i = 0; i < n; i++)
        A[i] = new number[n + 1];

    cout << "podaj macierz rozszerzona rownania" << endl;
    for(int i = 0; i < n; i++)
        for(int j = 0; j <= n; j++)
            cin >> A[i][j];

    cout << "\nmacierz rozszerzona ukladu" << endl;
    print_matrix(n, A);

    gaussian_elimination(n, A);

    number * X = new number[n]; //wektor niewiadomych

    calculate_variables(n, A, X);
    cout << "\nmacierz rozszerzona ukladu po eliminacji gaussa" << endl;
    print_matrix(n, A);
    cout << "\nwektor niewiadomych wyliczony na podstawie schodkowej powyzszej macierzy" << endl;
    print_vector(n, X);

    number * B = new number[n]; //wektor prawej strony wyliczony na podstawie wyliczonego wektora niewiadomych
    multiply(n, A, X, B);
    cout << "\nwektor prawej strony wyliczony na podstawie wektora niewiadomych" << endl;
    print_vector(n, B);

    number * Diff = new number[n]; //wektor roznic, miedzy wyliczonym, a wejsciowym wetorem prawej strony
    count_differences(n, A, B, Diff);
    cout << "\nroznice miedzy wejsciowym, a wyliczonym na podstawie wektore niewiadomych wektorem prawej strony" << endl;
    print_vector(n, Diff);

    number enorm = euclidean_norm(n, X);
    number mnorm = maximum_norm(n, X);

    cout<<"\nnorma euklidesowa wektora niewiadomych - "<<enorm<<"\nnorma maksimum wektora niewiadomych - "<<mnorm<<endl;
}


