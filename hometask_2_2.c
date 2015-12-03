//Напишите на языке C программу, вычисляющую определенный интеграл элементарной непрерывной на участке интегрирования функции
//с помощью методов левых прямоугольников. Программа должна вычислять значение интеграла в нескольких потоков.
//Количество потоков является задаваемым пользователем параметром.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct DATA
{
    double h;
    double a;
    double b;
    double summ;
};
typedef struct DATA DATA;

double f(double x){
    return x*x + 4*x + 3;
}

void *threadFunc(void *arg)
{
    DATA* a = (DATA*)arg;
    while(a->a +a->h < a->b){
        a->summ += f(a->a)*a->h;
        a->a += a->h;
    }
}

int main(void)
{
    double countOfThread;
    printf("count of threads : ");
    scanf("%lf", &countOfThread);
    pthread_t* ptr  = (pthread_t*) malloc(sizeof(pthread_t)*countOfThread);

    DATA **arg = (DATA**) malloc(sizeof(DATA*)*countOfThread);
    double h = 0.00000001;
	double a = 0; 
	double b = 2;
	int i;
	double step = (b-a)/countOfThread;
	for(i=0; i <countOfThread; ++i){
		arg[i] = (DATA*) malloc(sizeof(DATA));
		arg[i]->a = a;
		arg[i]->b = a+step-h/2;
		arg[i]->h = h;
		arg[i]->summ = 0;
		a+=step;
		pthread_create(&ptr[i],NULL, threadFunc, (void*)arg[i]);

   		pthread_join(ptr[i],NULL);
	}
	double summ = 0;
	i=0;
	for(; i < countOfThread; ++i)
		summ += arg[i]->summ;
    printf("summ = %lf\n", summ);
    return 0;
}
