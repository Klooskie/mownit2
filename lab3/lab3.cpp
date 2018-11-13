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

//f(x) = x^2 - 9
number f_function(number x){
    return x * x - 9;
}

//f'(x) = 2 * x
number f_derivative(number x){
    return 2 * x;
}

bool stopping_rule_1(number f3, number ro){
    if(absolute(f3) < ro)
        return true;
    return false;
}

bool stopping_rule_2(number x2, number x3, number ro){
    if(absolute(x2 - x3) < ro)
        return true;
    return false;
}

void secant_method(number a, number b, number ro, int stopping_rule_number){
    cout << endl;

    number x1, x2, x3, f1, f2, f3, tmp;
    int number_of_iterations = 0, max_iters = 1000;

    x1 = a;
    x2 = b;
    f1 = f_function(x1);
    f2 = f_function(x2);

    while(number_of_iterations < max_iters){

        if(absolute(f1 - f2) < 0.0000001){
            cout << "zle punkty startowe - sytuacja z praktycznie taka sama wartoscia funkcji" << endl;
        }

        x3 = x2;
        tmp = f2 * (x2 - x1);
        tmp /= (f2 - f1);
        x3 -= tmp;
        f3 = f_function(x3);

        if(stopping_rule_number == 1 && stopping_rule_1(f3, ro))
            break;

        if(stopping_rule_number == 2 && stopping_rule_2(x2, x3, ro))
            break;

        x1 = x2;
        f1 = f2;
        x2 = x3;
        f2 = f3;
        number_of_iterations++;
    }

    cout << "liczba iteracji " << number_of_iterations << "\tmiejsce zerowe " << x3 << endl;

}

void newtons_method(number a, number b, number ro, int stopping_rule_number){
    cout << endl;

    number x1, x2, f1, f_der_1, f2;
    int number_of_iterations = 0, max_iters = 1000;

    x1 = a;
    f1 = f_function(x1);

    while(number_of_iterations < max_iters){

        f_der_1 = f_derivative(x1);

        if(absolute(f_der_1) < 0.0000001){
            cout << "zly punkt startowy - zerowa pochodna" << endl;
        }

        x2 = x1;
        x2 -= (f1 / f_der_1);
        f2 = f_function(x2);

        if(stopping_rule_number == 1 && stopping_rule_1(f2, ro))
            break;

        if(stopping_rule_number == 2 && stopping_rule_2(x1, x2, ro))
            break;

        x1 = x2;
        f1 = f2;
        number_of_iterations++;
    }

    cout << "liczba iteracji " << number_of_iterations << "\tmiejsce zerowe " << x1 << endl;

}

int main(){

    number a, b, ro;
    int stopping_rule_number;
    cout << "wpisz krance przedzialu, ro z warunku stopu i numer warunku stopu" << endl;
    cin >> a >> b >> ro >> stopping_rule_number;
    secant_method(a, b, ro, stopping_rule_number);
    newtons_method(a, b, ro, stopping_rule_number);

}