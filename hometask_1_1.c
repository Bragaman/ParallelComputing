//Написать на языке C программу, запрашивающую у пользователя три натуральных числа:
//количество строк в матрице  A, количество столбцов в матрице A и количество столбцов в матрице B
//и реализующее умножение матриц  A и B . Матрицы заполняются случайными целыми двухзначными числами.

#include "stdio.h"
#include <time.h>
#include <stdlib.h>

long* createRandomMatrix(long n, long m){
  long *matrix = (long *)malloc(n * m * sizeof(long));
  long i=0;
  for(i; i < n; i++){
    long j=0;
    for(j; j < m; j++){
      long tmp = rand()%100;
      matrix[i*m + j] = (tmp < 10) ? tmp+=10 : tmp;

      // printf("%d %d %d; ",i, j, matrix[i*m + j] );
    }
  }
  // printf("\n\n");
  return matrix;
}

void printMatrix(long *matrix, long n, long m){
  long i=0;
  for(i; i < n; i++){
    long j=0;
    for(j; j < m; j++){
        printf("%d ", matrix[i*m + j]);
    }
    printf("\n");
  }
  printf("\n");
}

long calcItem(long* matrixA, long* matrixB, long m, long n, long q, long x, long y){
  long tmp=0;
  long i=0;
  for(i; i < n; i++){
    tmp+= matrixA[y*m+i]*matrixB[i*m+x];
  }
  return tmp;
}

void calc(long* matrixA, long* matrixB, long* matrixC, long m, long n, long q){
  long i=0;
  for(i; i < m; i++){
    long j=0;
    for(j; j < q; j++){
      matrixC[i*q + j] = calcItem(matrixA, matrixB, m,n,q, j, i);
    }
  }
}

long main(long argc, char const *argv[]) {

  long m=0 ,n=0 ,q=0 ; //first row, fisrt column, second column
  srand(time(NULL));
  printf("enter count of row first matrix: ");
  scanf("%d",&m );
  printf("enter count of column first matrix: ");
  scanf("%d",&n);
  printf("enter count of column second matrix: ");
  scanf("%d",&q );

  long* matrixA = createRandomMatrix(m, n);
  long* matrixB = createRandomMatrix(n, q);
  long* matrixC = (long*)malloc(m*q*sizeof(long));

  printMatrix(matrixA, m, n);
  printMatrix(matrixB, n, q);

  calc(matrixA, matrixB, matrixC, m, n, q);

  printMatrix(matrixC, m, q);
  return 0;
}
