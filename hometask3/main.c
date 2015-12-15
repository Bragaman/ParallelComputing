//Реализовать быструю сортировку массива целых чисел, используя OpenMP.
//В программе должны быть методы, позволяющие выполнить сортировку в последовательном режиме, в параллельном режиме,
//программа должна печатать время работы, потребовавшееся для сортировки массива.
//Массив заполняется случайными целыми числами.

#include <stdio.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>



void quickSortParallel(float* a, const long n) {
    long i = 0, j = n;
    float  pivot = a[n / 2]; // опорный элемент

    do {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;

        if (i <= j) {
            float tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++; j--;
        }
    } while (i <= j);

#pragma omp task shared(a)
    if (j > 0) quickSortParallel(a, j);
#pragma omp task shared(a)
    if (n > i) quickSortParallel(a + i, n - i);
#pragma omp taskwait
}

void quickSort(float* a, const long n) {
    long i = 0, j = n;
    float  pivot = a[n / 2]; // опорный элемент

    do {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;

        if (i <= j) {
            float tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++; j--;
        }
    } while (i <= j);


    if (j > 0) quickSort(a, j);
    if (n > i) quickSort(a + i, n - i);

}

float* createRandomMatrix(long n){
    float* matrix = (float*) malloc(n * sizeof(float));
    long  i=0;
    for(i; i < n; i++){
        float tmp = 1000000%n+i*n - 60*i*n;
        matrix[i] = tmp;
    }
    return matrix;
}

void printArray(float *matrix, long  size){
    long  i=0;
    for(i=0; i < size; ++i){
        printf("%f ", matrix[i]);
    }
}

int main(void)
{
    long n = 99900000;
     srand(time(NULL));
    float* a = createRandomMatrix(n);

    clock_t time;
    time = clock();
    quickSortParallel(a, n);
    time = clock() - time;
    printf("Parallel %f\n", (double)time/CLOCKS_PER_SEC);

    float *b = createRandomMatrix(n);
    time = clock();
    quickSort(b, n);
    time = clock() - time;
    printf("non  %f\n", (double)time/CLOCKS_PER_SEC);
//    printArray(a, n);
    return 0;
}

