//Написать на языке C программу, запрашивающую у пользователя три натуральных числа:
//количество строк в матрице  A, количество столбцов в матрице A и количество столбцов в матрице B
//и реализующее умножение матриц  A и B . Матрицы заполняются случайными целыми двухзначными числами.

#include "stdio.h"
#include <time.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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
                result[i*columnB + j] +=  A[i*columnA + k] * B[k*columnB + j];
        }
    }
    printMatrix(result, rowA, columnB);
}

int calcParallel(long *result, long *A, long *B, int rowA, int columnA, int columnB, int process )
{

    key_t key = 1995;

    pid_t* pid = (pid_t *) malloc(sizeof(pid_t) * process);
    int shmid;

    if ((shmid = shmget(key, rowA*columnB*sizeof(long), IPC_CREAT | 0666)) == -1){
        printf("Unsuccessful memory get \n");
        exit(-1);
    }
    int pr;



    for(pr = 0; pr < process; pr++) {
        pid[pr] = fork();
        if (pid[pr] == 0)
        {
            long* ans = (long *)shmat(shmid, NULL, 0);
            int i=pr;
            for(i; i < rowA; i+=process ) {
                int j = 0;
                for(j; j < columnB; j++) {
                    int k =0;
                    for(k; k < columnA; ++k)
                        ans[i*columnB + j] +=  A[i*columnA + k] * B[k*columnB + j];
                }
            }
            return 0;
        }
    }
    for(pr = 0; pr < process; pr++) {
        waitpid (pid[pr], NULL, 0);
    }





    if (pid !=0) {
        result = (long *) shmat(shmid, NULL, 0);
        printMatrix(result, rowA, columnB);
        if (shmctl(shmid, IPC_RMID, 0) < 0) {  /* This destroys the memory queue */
            printf("Problems destroying shared memory ID \n");
            exit(-1);
        }
    }
}

int main(long argc, char const *argv[]) {

    int rowA =0 , columnA =0 , columnB =0 ; //first row, fisrt column, second column
    int process;
    srand(time(NULL));
    printf("enter count of row first matrix: ");
    scanf("%d",&rowA);
    printf("enter count of column first matrix: ");
    scanf("%d",&columnA);
    printf("enter count of column second matrix: ");
    scanf("%d",&columnB);
    printf("enter count of process: ");
    scanf("%d", &process);




    long* matrixA = createRandomMatrix(rowA, columnA);
    long* matrixB = createRandomMatrix(columnA, columnB);
    long* matrixC = (long*)malloc(rowA * columnB * sizeof(long));
    int i;
    for(i = 0; i< rowA * columnB; ++i )
        matrixC[i] = 0;

    printMatrix(matrixA, rowA, columnA);
    printMatrix(matrixB, columnA, columnB);

    if(process < 1){
        printf("i think that user say 1 process");
        calc(matrixC, matrixA, matrixB, rowA, columnA, columnB);
    }
    if(process == 1)
        calc(matrixC, matrixA, matrixB, rowA, columnA, columnB);

    calcParallel(matrixC, matrixA, matrixB, rowA, columnA, columnB, process);




    return 0;
}
