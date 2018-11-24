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


number f1_fun(number x1, number x2, number x3){
    return pow(x1, 2) + pow(x2, 2) + x3 - 1;
}
number f1_der_x1(number x1, number x2, number x3){
    return 2 * x1;
}
number f1_der_x2(number x1, number x2, number x3){
    return 2 * x2;
}
number f1_der_x3(number x1, number x2, number x3){
    return 1;
}

number f2_fun(number x1, number x2, number x3){
    return 2 * pow(x1, 2) + pow(x2, 2) + pow(x3, 3) - 2;
}
number f2_der_x1(number x1, number x2, number x3){
    return 4 * x1;
}
number f2_der_x2(number x1, number x2, number x3){
    return 2 * x2;
}
number f2_der_x3(number x1, number x2, number x3){
    return 3 * pow(x3, 2);
}

number f3_fun(number x1, number x2, number x3){
    return 3 * x1 - 2 * pow(x2, 3) - 2 * pow(x3, 2) - 3;
}
number f3_der_x1(number x1, number x2, number x3){
    return 3;
}
number f3_der_x2(number x1, number x2, number x3){
    return (-6) * pow(x2, 2);
}
number f3_der_x3(number x1, number x2, number x3){
    return (-4) * x3;
}

void print_vector(number * x){
    for(int i = 0; i < 3; i++)
        cout << x[i] << endl;
    cout << endl;
}

number absolute(number x){
    return x < 0 ? (-1) * x : x;
}

number * multiply_matrix_and_vector(number ** matrix, number * vector){
    number * result = new number [3];
    for(int i = 0; i < 3; i++){
        result[i] = 0;
        for(int j = 0; j < 3; j++)
            result[i] += matrix[i][j] * vector[j];
    }
    return result;
}

number * subtract_vectors(number * x1, number * x2){
    number * result = new number [3];
    for(int i = 0; i < 3; i++){
        result[i] = x1[i] - x2[i];
    }
    return result;
}

number maximum_norm(number * x){
    number max_number = 0;
    for(int i = 0; i < 3; i++){
        if(absolute(x[i]) > max_number)
            max_number = absolute(x[i]);
    }
    return max_number;
}

number * F(number * x){
    number * result = new number [3];
    result[0] = f1_fun(x[0], x[1], x[2]);
    result[1] = f2_fun(x[0], x[1], x[2]);
    result[2] = f3_fun(x[0], x[1], x[2]);
    return result;
}

number * find_matching_proper_solution(number * x){
    number * proper_solution_1 = new number [3];
    number * proper_solution_2 = new number [3];
    number * proper_solution_3 = new number [3];

    proper_solution_1[0] = 1;
    proper_solution_1[1] = -1;
    proper_solution_1[2] = -1;

    proper_solution_2[0] = 1;
    proper_solution_2[1] = 0;
    proper_solution_2[2] = 0;

    proper_solution_3[0] = 0.953156;
    proper_solution_3[1] = -0.428689;
    proper_solution_3[2] = -0.0922802;

    number diff_1 = maximum_norm(subtract_vectors(x, proper_solution_1));
    number diff_2 = maximum_norm(subtract_vectors(x, proper_solution_2));
    number diff_3 = maximum_norm(subtract_vectors(x, proper_solution_3));

    if(diff_1 <= diff_2 && diff_1 <= diff_3)
        return proper_solution_1;

    if(diff_2 <= diff_3)
        return proper_solution_2;

    return proper_solution_3;
}

number * calculate_d(number * x){
    
    number * minus_F = F(x);
    for(int i = 0; i < 3; i++)
        minus_F[i] *= -1;

    number ** jacobian = new number * [3];
    number ** inverse_jacobian = new number * [3];
    for(int i = 0; i < 3; i++){
        jacobian[i] = new number [3];
        inverse_jacobian[i] = new number [3];
    }

    jacobian[0][0] = f1_der_x1(x[0], x[1], x[2]);
    jacobian[0][1] = f1_der_x2(x[0], x[1], x[2]);
    jacobian[0][2] = f1_der_x3(x[0], x[1], x[2]);

    jacobian[1][0] = f2_der_x1(x[0], x[1], x[2]);
    jacobian[1][1] = f2_der_x2(x[0], x[1], x[2]);
    jacobian[1][2] = f2_der_x3(x[0], x[1], x[2]);

    jacobian[2][0] = f3_der_x1(x[0], x[1], x[2]);
    jacobian[2][1] = f3_der_x2(x[0], x[1], x[2]);
    jacobian[2][2] = f3_der_x3(x[0], x[1], x[2]);

    number determinant, inverse_determinant;

    determinant = jacobian[0][0] * jacobian[1][1] * jacobian[2][2];
    determinant += jacobian[1][0] * jacobian[2][1] * jacobian[0][2];
    determinant += jacobian[2][0] + jacobian[0][1] * jacobian[1][2];
    determinant -= jacobian[0][0] * jacobian[2][1] * jacobian[1][2];
    determinant -= jacobian[2][0] * jacobian[1][1] * jacobian[0][2];
    determinant -= jacobian[1][0] * jacobian[0][1] * jacobian[2][2];

    inverse_determinant = 1 / determinant;

    inverse_jacobian[0][0] = inverse_determinant * (jacobian[1][1] * jacobian[2][2] - jacobian[1][2] * jacobian[2][1]);
    inverse_jacobian[0][1] = inverse_determinant * (jacobian[0][2] * jacobian[2][1] - jacobian[0][1] * jacobian[2][2]);
    inverse_jacobian[0][2] = inverse_determinant * (jacobian[0][1] * jacobian[1][2] - jacobian[0][2] * jacobian[1][1]);
    
    inverse_jacobian[1][0] = inverse_determinant * (jacobian[1][2] * jacobian[2][0] - jacobian[1][0] * jacobian[2][2]);
    inverse_jacobian[1][1] = inverse_determinant * (jacobian[0][0] * jacobian[2][2] - jacobian[0][2] * jacobian[2][0]);
    inverse_jacobian[1][2] = inverse_determinant * (jacobian[0][2] * jacobian[1][0] - jacobian[0][0] * jacobian[1][2]);

    inverse_jacobian[2][0] = inverse_determinant * (jacobian[1][0] * jacobian[2][1] - jacobian[1][1] * jacobian[2][0]);
    inverse_jacobian[2][1] = inverse_determinant * (jacobian[0][1] * jacobian[2][0] - jacobian[0][0] * jacobian[2][1]);
    inverse_jacobian[2][2] = inverse_determinant * (jacobian[0][0] * jacobian[1][1] - jacobian[0][1] * jacobian[1][0]);

    return multiply_matrix_and_vector(inverse_jacobian, minus_F);

}

bool stopping_rule_1(number * x1, number * x2, number ro){
    if(maximum_norm(subtract_vectors(x1, x2)) < ro)
        return true;
    return false;
}

bool stopping_rule_2(number * x, number ro){
    if(maximum_norm(F(x)) < ro)
        return true;
    return false;
}

void newtons_method(number * start, number ro, int stopping_rule_number){
    
    number * x1 = new number [3]; 
    number * x2 = new number [3];
    number * d;

    for(int i = 0; i < 3; i++)
        x1[i] = start[i];

    int number_of_iterations = 0, max_iters = 1000000;

    while(number_of_iterations < max_iters){

        d = calculate_d(x1);

        for(int i = 0; i < 3; i++)
            x2[i] = x1[i] + d[i];

        if(stopping_rule_number == 1 && stopping_rule_1(x1, x2, ro))
            break;

        if(stopping_rule_number == 2 && stopping_rule_2(x1, ro))
            break;

        free(d);
        for(int i = 0; i < 3; i++)
            x1[i] = x2[i];
        number_of_iterations++;

    }

    if(number_of_iterations == max_iters){
        cout << "Przekroczono maksymalna liczbe iteracji w metodzie newtona, prawdopodobnie zle dobrano punkt startowy" << endl;
        return;
    }

    cout << "liczba iteracji " << number_of_iterations << endl;
    cout << "wyliczony wektor:" << endl;
    cout << "[" << x1[0] << ", " << x1[1] << ", " << x1[2] << "]" << endl;

    number * proper_solution = find_matching_proper_solution(x1);
    cout << "delta " << maximum_norm(subtract_vectors(x1, proper_solution)) << endl;
}

int main(){

    number * start = new number [3];

    start[0] = 1;
    start[1] = 0;
    start[2] = 0;

    for(int stopping_rule_number = 1; stopping_rule_number <= 2; stopping_rule_number++){

        cout << "\n\n\nwarunek stopu " << stopping_rule_number << endl;

        for(number ro = 0.0001; ro >= 0.0000000001; ro *= 0.000001){
            cout << "\nro: " << ro << endl;
            newtons_method(start, ro, stopping_rule_number);
        }
    }

}