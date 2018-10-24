#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
typedef float number;

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

void zad1(){

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
            A[i][j] = ((number) 1) / ((number) i + (number) j + 1); //+1 bo indeksy od 0
        }
    }

    number * X = new number[n];
    // srand(time(NULL));
    for(int i = 0; i < n; i++){
        if(rand() % 2)
            X[i] = 1;
        else
            X[i] = -1;
    }

    cout << "\nwektor niewiadomych - x" <<endl;
    print_vector(n, X);

    number * B = new number[n];
    multiply(n, A, X, B);

    cout << "\nwektor prawej strony - b" << endl;
    print_vector(n, B);

    for(int i = 0; i < n; i++){
        A[i][n] = B[i];
    }

    // cout << "\nmacierz" << endl;
    // print_matrix(n, A);

    gaussian_elimination(n, A);
    // cout << "\nmacierz po eliminacji" << endl;
    // print_matrix(n, A);

    number * X2 = new number[n];
    calculate_variables(n, A, X2);

    cout << "\nwyliczony wektor niewiadomych" << endl;
    print_vector(n, X2);

    cout << "\nnorma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X2) <<endl;
    cout << "roznica norm euklidesowych " << absolute(euclidean_norm(n, X) - euclidean_norm(n, X2)) << endl;

    cout << "\nnorma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X2) <<endl;
    cout << "roznica norm maksimum " << absolute(maximum_norm(n, X) - maximum_norm(n, X2)) << endl;
}

void zad2(){

    int n;
    cout << "wpisz rozmiar macierzy" << endl;
    cin >> n;

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

    number * X = new number[n];
    // srand(time(NULL));
    for(int i = 0; i < n; i++){
        if(rand() % 2)
            X[i] = 1;
        else
            X[i] = -1;
    }

    cout << "\nwektor niewiadomych - x" <<endl;
    print_vector(n, X);

    number * B = new number[n];
    multiply(n, A, X, B);

    cout << "\nwektor prawej strony - b" << endl;
    print_vector(n, B);

    for(int i = 0; i < n; i++){
        A[i][n] = B[i];
    }

    // cout << "\nmacierz" << endl;
    // print_matrix(n, A);

    gaussian_elimination(n, A);
    // cout << "\nmacierz po eliminacji" << endl;
    // print_matrix(n, A);

    number * X2 = new number[n];
    calculate_variables(n, A, X2);

    cout << "\nwyliczony wektor niewiadomych" << endl;
    print_vector(n, X2);

    cout << "\nnorma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X2) <<endl;
    cout << "roznica norm euklidesowych " << absolute(euclidean_norm(n, X) - euclidean_norm(n, X2)) << endl;

    cout << "\nnorma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X2) <<endl;
    cout << "roznica norm maksimum " << absolute(maximum_norm(n, X) - maximum_norm(n, X2)) << endl;
}

void zad3_gauss(int n, number ** A, number * X, number * X2){

    //metoda eliminacji gaussa
    gaussian_elimination(n, A);
    
    calculate_variables(n, A, X2);
}

void zad3_thomas(int n, number ** A2, number * X, number * X3){

    //metoda przeganiania
    number * B = new number[n]; 
    B[0] = (- A2[0][1]) / A2[0][0];

    number * Y = new number[n];
    Y[0] = A2[0][n] / A2[0][0];

    for(int i = 1; i < n; i++){ //B[n-1] nie jest mi potrzebne, ale Y[n-1] ma taka sama wartosc co X[n-1]
        B[i] = (- A2[i][i+1])/(A2[i][i-1] * B[i-1] + A2[i][i]);
        Y[i] = (A2[i][n] - (A2[i][i-1] * Y[i-1]))/(A2[i][i-1] * B[i-1] + A2[i][i]);
    } 

    X3[n-1] = Y[n-1];

    for(int i = n - 2; i >= 0; i--){
        X3[i] = B[i] * X3[i+1] + Y[i];
    }
}

void time_printer(long int time_start, long int time_end){
    cout << "czas trwania metody: " << ((double)(time_end - time_start))/CLOCKS_PER_SEC << endl;
}

void zad3(){

    int n;
    cout << "wpisz rozmiar macierzy" << endl;
    cin >> n;

    //alokacja macierzy
    number ** A = new number * [n];
    number ** A2 = new number * [n];
    for(int i = 0; i < n; i++){
        A[i] = new number[n + 1];
        A2[i] = new number[n + 1];
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

    //utworzenie wektora niewiadomych
    number * X = new number[n];
    number * X2 = new number[n];
    number * X3 = new number[n];

    srand(time(NULL));
    for(int i = 0; i < n; i++){
        if(rand() % 2)
            X[i] = 1;
        else
            X[i] = -1;
    }

    // cout << "\nwektor niewiadomych - x" <<endl;
    // print_vector(n, X);

    //wyliczenie wektora prawej strony - b
    number * b = new number[n];
    multiply(n, A, X, b);
    for(int i = 0; i < n; i++){
        A[i][n] = b[i];
    }

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n + 1; j++)
            A2[i][j] = A[i][j];

    long int time_start, time_end;

    time_start = clock();
    zad3_gauss(n, A, X, X2);
    time_end = clock();

    cout << "\n\nEliminacja Gaussa:" << endl;
    time_printer(time_start, time_end);

    cout << "\nnorma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X2) <<endl;
    cout << "roznica norm euklidesowych " << absolute(euclidean_norm(n, X) - euclidean_norm(n, X2)) << endl;

    cout << "\nnorma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X2) <<endl;
    cout << "roznica norm maksimum " << absolute(maximum_norm(n, X) - maximum_norm(n, X2)) << endl;

    time_start = clock();
    zad3_thomas(n, A2, X, X3);
    time_end = clock();

    cout << "\n\nAlgorytm Thomasa:" << endl;
    time_printer(time_start, time_end);

    cout << "\nnorma euklidesowa wektora zadanego " << euclidean_norm(n, X) << endl;
    cout << "norma euklidesowa wektora obliczonego " << euclidean_norm(n, X3) <<endl;
    cout << "roznica norm euklidesowych " << absolute(euclidean_norm(n, X) - euclidean_norm(n, X3)) << endl;

    cout << "\nnorma maksimum wektora zadanego " << maximum_norm(n, X) << endl;
    cout << "norma maksimum wektora obliczonego " << maximum_norm(n, X3) <<endl;
    cout << "roznica norm maksimum " << absolute(maximum_norm(n, X) - maximum_norm(n, X3)) << endl;

}

int main(){

    // zad1();
    // zad2();
    zad3();

}


