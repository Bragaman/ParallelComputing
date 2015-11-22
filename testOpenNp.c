//gcc -fopenmp -o omp omp.c
//export OMP_NUM_THREADS=8 //указывает количесвто потоков
// ps -eLo pid,tid,psr,args|grep ИМЯ программы 

#include <stdio.h>
#include <omp.h>
#include <time.h>

int main (int argc, char *argv[]){
  #pragma omp parallel num_threads(6) //можно задать колво потоков через num_threads
  {
    printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
    nanosleep(&(struct timespec){.tv_sec = 30}, NULL);
    printf("thread finish");
  }
  return 0;
}
