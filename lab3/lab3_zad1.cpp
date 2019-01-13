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


number proper_zero_of_f = 0.522247;

number absolute(number x){
    return x < 0 ? (-1) * x : x;
}

//f(x) = (x - 1) * e ^ (-16 * x) + x ^ 14
//f(x) = 0 dla x = 0.522247 
number f_function(number x){

    number f = exp((-16) * x);
    f *= (x - 1);
    f += pow(x, 14);
    return f;
    
}

//f'(x) = 14 * x ^ 13 + (17 - 16x) * e ^ (-16x) 
number f_derivative(number x){
    
    number f = exp((-16) * x);
    f *= (17 - (16 * x));
    f += 14 * pow(x, 13);
    return f;

}

bool stopping_rule_1(number x1, number x2, number ro){
    if(absolute(x1 - x2) < ro)
        return true;
    return false;
}

bool stopping_rule_2(number f, number ro){
    if(absolute(f) < ro)
        return true;
    return false;
}

void secant_method(number start1, number start2, number ro, int stopping_rule_number){
    cout << endl << "Metoda siecznych\nx1 = " << start1 << "   x2 = " << start2 << "   ro = " << ro << endl;

    number x1, x2, x3, f1, f2, f3, tmp;
    int number_of_iterations = 0, max_iters = 10000;

    x1 = start1;
    x2 = start2;
    f1 = f_function(x1);
    f2 = f_function(x2);

    while(number_of_iterations < max_iters){
        number_of_iterations++;

        x3 = x2;
        tmp = f2 * (x2 - x1);
        tmp /= (f2 - f1);
        x3 -= tmp;
        f3 = f_function(x3);

        if(stopping_rule_number == 1 && stopping_rule_1(x2, x3, ro))
            break;

        if(stopping_rule_number == 2 && stopping_rule_2(f3, ro))
            break;

        x1 = x2;
        f1 = f2;
        x2 = x3;
        f2 = f3;
    }

    if(number_of_iterations == max_iters){
        cout << "Przekroczono maksymalna liczbe iteracji w metodzie siecznych, prawdopodobnie zle dobrano punkty startowe" << endl;
        return;
    }

    cout << "liczba iteracji " << number_of_iterations << "   miejsce zerowe " << x3 << "   delta " << absolute(proper_zero_of_f - x3) << endl;

}

void newtons_method(number start1, number ro, int stopping_rule_number){
    cout << endl << "Metoda newtona\nx1 = " << start1 << "   ro = " << ro << endl;

    number x1, x2, f1, f_der_1, f2;
    int number_of_iterations = 0, max_iters = 10000;

    x1 = start1;
    f1 = f_function(x1);

    while(number_of_iterations < max_iters){

        f_der_1 = f_derivative(x1);

        x2 = x1;
        x2 -= (f1 / f_der_1);
        f2 = f_function(x2);

        if(stopping_rule_number == 1 && stopping_rule_1(x1, x2, ro))
            break;

        if(stopping_rule_number == 2 && stopping_rule_2(f1, ro))
            break;

        x1 = x2;
        f1 = f2;
        number_of_iterations++;
    }

    if(number_of_iterations == max_iters){
        cout << "Przekroczono maksymalna liczbe iteracji w metodzie newtona, prawdopodobnie zle dobrano punkt startowy" << endl;
        return;
    }

    cout << "liczba iteracji " << number_of_iterations << "   miejsce zerowe " << x1 << "   delta " << absolute(proper_zero_of_f - x1) << endl;

}

int main(){

    number a, b, ro, start1, start2;
    int stopping_rule_number;
    a = 0.2;
    b = 2.5;

    for(stopping_rule_number = 1; stopping_rule_number <= 2; stopping_rule_number++){

        cout << "\n\n\nwarunek stopu " << stopping_rule_number << endl;

        for(ro = 0.0001; ro >= 0.0000000001; ro *= 0.001){
            for(start1 = b; start1 > a - 0.01; start1 -= 0.1){
                newtons_method(start1, ro, stopping_rule_number);
            }
        
            cout << "\n\n\n";

            start1 = a;
            for(start2 = b; start2 > a + 0.01; start2 -= 0.1){
                secant_method(start1, start2, ro, stopping_rule_number);
            }

            cout << "\n\n\n";

            start1 = b;
            for(start2 = b - 0.1; start2 > a - 0.01; start2 -= 0.1){
                secant_method(start1, start2, ro, stopping_rule_number);
            }

            cout << "\n\n\n";

        }
    }
}
