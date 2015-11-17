#include <pthread.h>
#include <stdio.h>


struct DATA
{
    double h;
    double a;
    double b;
    double summ;
};
typedef struct DATA DATA;

double f(double x){
    double tmp =  4;
//    printf("f(%f) = %f\n", x, tmp);
    return tmp;
}

void *threadFunc(void *arg)
{
    double summ =0;
    DATA* a = (DATA*)arg;
    while(a->a +a->h < a->b){

        summ += f(a->a)*a->h;

        a->a += a->h;
//        printf("%d ", a->a);
    }
    a->summ = summ;
}

int main(void)
{
    pthread_t pth1;	// this is our thread identifier
    pthread_t pth2;
    int countOfThread =2;
    pthread_t* ptr  = (pthread_t*) malloc(sizeof(pthread_t)*countOfThread);
    DATA *arg1 = (DATA*) malloc(sizeof(DATA));
    DATA *arg2 = (DATA*) malloc(sizeof(DATA));
    double h = 0.00000001;

    arg1->a = 1; arg1->b = 2-h/2; arg1->h = h; arg1->summ = 0;
    arg2->a = 2; arg2->b = 3; arg2->h = h; arg2->summ = 0;

    pthread_create(&pth1,NULL, threadFunc, (void*)arg1);
    pthread_create(&pth2,NULL,threadFunc,(void*)arg2);

    pthread_join(pth1,NULL);
    pthread_join(pth2,NULL);



    printf("finish; summ = %f\n", arg1->summ+arg2->summ);

//    printf("finish; f = %d\n", f(2));

    return 0;
}
