//Написать на языке C программу, запрашивающую у пользователя три натуральных числа:
//количество строк в матрице  A, количество столбцов в матрице A и количество столбцов в матрице B
//и реализующее умножение матриц  A и B . Матрицы заполняются случайными целыми двухзначными числами.

#include "stdio.h"
#include <time.h>
#include <stdlib.h>

int* createRandomMatrix(int n, int m){
  int *matrix = (int *)malloc(n * m * sizeof(int));
  int i=0;
  for(i; i < n; i++){
    int j=0;
    for(j; j < m; j++){
      matrix[i*m + j] = rand()%2;
      // printf("%d %d %d; ",i, j, matrix[i*m + j] );
    }
  }
  // printf("\n\n");
  return matrix;
}

void printMatrix(int *matrix, int n, int m){
  int i=0;
  for(i; i < n; i++){
    int j=0;
    for(j; j < m; j++){
        printf("%d ", matrix[i*m + j]);
    }
    printf("\n");
  }
  printf("\n");
}

int calcItem(int* matrixA, int* matrixB, int m, int n, int q, int x, int y){
  int tmp=0;
  int i=0;
  for(i; i < n; i++){
    tmp+= matrixA[y*m+i]*matrixB[i*m+x];
  }
  return tmp;
}

void calc(int* matrixA, int* matrixB, int* matrixC, int m, int n, int q){
  int i=0;
  for(i; i < m; i++){
    int j=0;
    for(j; j < q; j++){
      matrixC[i*q + j] = calcItem(matrixA, matrixB, m,n,q, j, i);
    }
  }
}

int main(int argc, char const *argv[]) {

  int m=0 ,n=0 ,q=0 ; //first row, fisrt column, second column
  srand(time(NULL));
  printf("enter count of row first matrix: ");
  scanf("%d",&m );
  printf("enter count of column first matrix: ");
  scanf("%d",&n);
  printf("enter count of column second matrix: ");
  scanf("%d",&q );

  int* matrixA = createRandomMatrix(m, n);
  int* matrixB = createRandomMatrix(n, q);
  int* matrixC = (int*)malloc(m*q*sizeof(int));

  printMatrix(matrixA, m, n);
  printMatrix(matrixB, n, q);

  calc(matrixA, matrixB, matrixC, m, n, q);

  printMatrix(matrixC, m, q);
  return 0;
}
