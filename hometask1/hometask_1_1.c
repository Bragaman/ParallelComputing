//Написать на языке C программу, запрашивающую у пользователя три натуральных числа:
//количество строк в матрице  A, количество столбцов в матрице A и количество столбцов в матрице B
//и реализующее умножение матриц  A и B . Матрицы заполняются случайными целыми двухзначными числами.

#include "stdio.h"
#include <time.h>
#include <stdlib.h>

long* createRandomMatrix(int n, int m){
    long *matrix = (long *)malloc(n * m * sizeof(long));
    int i=0;
    for(i; i < n; i++) {
        int j = 0;
        for (j; j < m; j++)
            matrix[i * m + j] = rand() % 90 +10;
    }
    return matrix;
}

void printMatrix(long *matrix, int n, int m){
    int i=0;
    for(i; i < n; i++){
        int j=0;
        for(j; j < m; j++)
            printf("%ld ", matrix[i*m + j]);

        printf("\n");
    }
    printf("\n");
}

void calc(long *result, long *A, long *B, int rowA, int columnA, int columnB )
{
    int i=0;
    for(i; i < rowA; ++i ) {
        int j = 0;
        for(j; j < columnB; ++j) {
            int k =0;
            for(k; k < columnA; ++k)
                result[i*columnB + j] +=  A[i*columnA + k] + B[k*columnB + j];
        }
    }
}

int main(long argc, char const *argv[]) {

    int rowA =0 , columnA =0 , columnB =0 ; //first row, fisrt column, second column
    srand(time(NULL));
    printf("enter count of row first matrix: ");
    scanf("%d",&rowA);
    printf("enter count of column first matrix: ");
    scanf("%d",&columnA);
    printf("enter count of column second matrix: ");
    scanf("%d",&columnB);

    long* matrixA = createRandomMatrix(rowA, columnA);
    long* matrixB = createRandomMatrix(columnA, columnB);
    long* matrixC = (long*)malloc(rowA * columnB * sizeof(long));
    int i;
    for(i = 0; i< rowA * columnB; ++i )
        matrixC[i] = 0;

    printMatrix(matrixA, rowA, columnA);
    printMatrix(matrixB, columnA, columnB);

    calc(matrixC, matrixA, matrixB, rowA, columnA, columnB);

    printMatrix(matrixC, rowA, columnB);
    return 0;
}
