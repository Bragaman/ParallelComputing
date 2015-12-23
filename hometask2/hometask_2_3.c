// Напишите на языке C программу, вычисляющую определенный интеграл
// элементарной непрерывной на участке интегрирования функции с помощью методов левых прямоугольников. 
// Программа должна вычислять значение интеграла в нескольких процессах.
// Количество процессов является задаваемым пользователем параметром.

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

double A = 1.0;
double B = 10000.0;
double *sum;
double sum1 = 0;
double sum2 = 0;
double step = 0.00001;
double N;

double func(double x) {
    return x * x - 4;
}

struct data {
    int id;
};

typedef struct data Data;

void integral(int id, double* sum)
{
    double i;
    // printf("%d\n", id);
    double temp1 = A + id * (B - A) / N;
    double temp2 = A + (id + 1) * (B - A) / N;
    double tempStep = step;
    double tempSum = 0;

    for(i = temp1; i < temp2; i += tempStep) {
        tempSum += func(i) * tempStep;
    }
    sum[id] = tempSum;
}

int main(void)
{
    int i = 0;
    key_t key = 2323;
    printf("Count of process: ");
    scanf("%lf", &N);

    pid_t* pid = malloc(sizeof(pid_t) * N);
    int shmid = shmget(key, (int)N * sizeof(double), IPC_CREAT | 0666);
    double* sum = (double *)shmat(shmid, NULL, 0);
    printf("\n");
    for(i = 0; i < N; i++) {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            double* ans = (double *)shmat(shmid, NULL, 0);
            integral(i, ans);
            return 0;
        }
    }
    for(i = 0; i < N; i++) {
        waitpid (pid[i], NULL, 0);
    }

    double ans = 0;
    for(i = 0; i < (int)N; i++) {
        ans += sum[i];
    }
    printf("Integral = %f\n",ans);
    return 0;
}

