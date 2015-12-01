#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

const double l = -4.0;
const double r = 4.0;
const int n_steps = 100000000;

double f(double x){
  return exp(-x * x);
}

double integrate(double (*func)(double), double a, double b, int n){
  int i;
  double h = (b - a) / n;
  double sum = 0.0;
  for(i = 0; i < n; i ++)
    sum += func(a + h * (i + 0.5));
  sum *= h;
  return sum;
}

double integrate_omp(double (*func)(double), double a, double b, int n){
  double h = (b - a) / n;
  double sum = 0.0;
  #pragma omp parallel
  {
    int i;
    int n_threads = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    int items_per_thread = n / n_threads;
    int left = thread_num * items_per_thread;
    int right = (thread_num == n_threads - 1) ? (n - 1) : (left + items_per_thread - 1);
    double sumloc = 0.0;
    for(i = left; i <= right; i++){
      sumloc += func(a + h * (i + 0.5));
    }
    #pragma omp atomic
    sum += sumloc;
  }
  sum *= h;
  return sum;
}

double integrate_omp_for(double (*func)(double), double a, double b, int n){
  double h = (b - a) / n;
  double sum = 0.0;
  #pragma omp parallel
  {
    double sumloc = 0.0;
    #pragma omp for
    for(int i = 0; i < n; i++){
      sumloc += func(a + h * (i + 0.5));
    }
    #pragma omp atomic
    sum += sumloc;
  }
  sum *= h;
  return sum;
}



double run_serial(){
  double t=omp_get_wtime();
  double res=integrate(f, l, r, n_steps);
  printf("SERIAL: %.12lf\t%.12lf\n", res, fabs(res - sqrt(M_PI)));
  return omp_get_wtime() - t;
}



double run_parrallel(){
  double t=omp_get_wtime();
  double res=integrate_omp(f, l, r, n_steps);
  printf("PARALLEL: %.12lf\t%.12lf\n", res, fabs(res - sqrt(M_PI)));
  return omp_get_wtime() - t;
}

int main(int argc, int *argv[]){
  double t1,t2;
  t1=run_serial();
  t2=run_parrallel();
  printf("%lf %lf %lf\n", t1, t2, t1 / t2);
  return 0;
}
