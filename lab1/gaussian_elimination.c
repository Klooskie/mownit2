#include <stdio.h>
#include <stdlib.h>

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

void elimination(){



    printf("XD");
}

int main(){

    number **A;
    number *B;
    int n;

    scanf("%d", &n);

    A = calloc(n, sizeof(number*));
    for(int i = 0; i < n; i++){
        A[i] = calloc(n, sizeof(number));
    }

    printf("wpisz kolejno wiersze tablicy\n");

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%lf", &A[i][j]);
        
    B = calloc(n, sizeof(number));

    printf("wpisz wartosci wektora prawej strony\n");

    for(int i = 0; i < n; i++)
        scanf("%lf", &B[i]);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%lf ", A[i][j]);
        }
        printf(" x%d  %lf\n", i, B[i]);
    }

}


