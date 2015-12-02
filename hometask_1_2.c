//Написать на языке C программу, запрашивающую у пользователя три натуральных числа:
//количество строк в матрице  A, количество столбцов в матрице A и количество столбцов в матрице B
//и реализующее умножение матриц  A и B . Матрицы заполняются случайными целыми двухзначными числами.
//Программа должна вычислять результирующую матрицу параллельно в нескольких процессах.
//Количество процессов является параметром в программе и может быть легко изменено.
//Матрицы должны хранится в разделяемой памяти.

#include <time.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_MODE (SHM_W | SHM_R |  IPC_CREAT)

int* createRandomMatrix(int n, int m){
  int *matrix = (int *)malloc(n * m * sizeof(int));
  int i=0;
  for(i; i < n; i++){
    int j=0;
    for(j; j < m; j++){
      matrix[i*m + j] = rand()%90 + 10;
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

void calc(int* matrixA, int* matrixB, int* matrixC, int m, int n, int q, int processCount, int id){
  int i=id;
  for(i; i < m; i+=processCount){
    int j=0;
    for(j; j < q; j++){
      matrixC[i*q + j] = calcItem(matrixA, matrixB, m,n,q, j, i);
    }
  }
}

int main(int argc, char const *argv[]) {

  int m=0 ,n=0 ,q=0, process=0; 
  //first row, fisrt column, second column, count of process
  srand(time(NULL));
  printf("enter count of row first matrix: ");
  scanf("%d", &m);
  printf("enter count of column first matrix: ");
  scanf("%d", &n);
  printf("enter count of column second matrix: ");
  scanf("%d", &q);
  printf("enter count of process: ");
  scanf("%d", &process);

  int* matrixA = createRandomMatrix(m, n);
  int* matrixB = createRandomMatrix(n, q);

  printMatrix(matrixA, m, n);
  printMatrix(matrixB, n, q);

  
  key_t key = 1995;
  pid_t* pid = malloc(sizeof(pid_t) * process);
  int shmid;

  if ((shmid = shmget(key, m*q*sizeof(int), IPC_CREAT | 0666)) == -1){ 
    printf("Unsuccessful memory get \n");
    exit(-1);
  }

  int i;
   for(i = 0; i < process; i++) {
      pid[i] = fork();
      if (pid[i] == 0)
      {
        int* ans = (int *)shmat(shmid, NULL, 0);
        calc( matrixA, matrixB, ans, m,n,q, process, i);
        return 0;
      }
   }
  for(i = 0; i < process; i++) {
      waitpid (pid[i], NULL, 0);
  }

  int* matrixC = (int*)shmat(shmid, NULL, 0);
  printMatrix(matrixC, m, q);
  if (pid !=0)
     if (shmctl(shmid,IPC_RMID,0) < 0) 
      {  /* This destroys the memory queue */
        printf("Problems destroying shared memory ID \n");
        exit(-1);
      }
  return 0;
}
