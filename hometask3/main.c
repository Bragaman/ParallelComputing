//Реализовать быструю сортировку массива целых чисел, используя OpenMP.
//В программе должны быть методы, позволяющие выполнить сортировку в последовательном режиме, в параллельном режиме,
//программа должна печатать время работы, потребовавшееся для сортировки массива.
//Массив заполняется случайными целыми числами.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


void changeCountOfThread(int count, int add)
{
#pragma omp critical
    {
        count += add;
    }
}
bool canMakeNewThread(int count)
{
    bool can = true;
#pragma omp critical
    {

        if (count  > 3)
            can = false;
    }
    return can;
}

void quickSort(int* arr, int first, int last)
{
    int i = first, j = last, x = arr[(first + last) / 2];

    do {
        while (arr[i] < x) i++;
        while (arr[j] > x) j--;

        if(i <= j) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        quickSort(arr, i, last);
    if (first < j)
        quickSort(arr, first, j);
}

void quickSortParallel(int* arr, int first, int last, int count)
{

    int i = first, j = last, x = arr[(first + last) / 2];

    do {
        while (arr[i] < x) i++;
        while (arr[j] > x) j--;

        if(i <= j) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
            i++;
            j--;
        }
    } while (i <= j);
    if((last - first > 1000) && canMakeNewThread(count)) {
        changeCountOfThread(count, 1);
        #pragma omp parallel sections
        {
            #pragma omp section
            quickSortParallel(arr, i, last, count);
            #pragma omp section
            quickSortParallel(arr, first, j, count);
        }
//        changeCountOfThread(count, -1 );
    } else {
        if (i < last)
            quickSortParallel(arr, i, last, count);
        if (first < j)
            quickSortParallel(arr, first, j, count);
    }
}


int validateArray(int size, int *arr)
{
    int i;
    for( i = 0; i < size-1; ++i)
        if(arr[i] > arr[i+1]) {
            printf("error!!\n");
            return 1;
        }
    printf("array is sorted\n");
    return 0;
}


int main(void)
{
    int i;
//    int N = 18000000;
    int N = 50000000;
    int *arr;
    arr = (int *) malloc(sizeof(int) * N);
    for(i = 0; i < N; i++) {
        arr[i] = rand() % 10000;
    }
    double t1, t2;
    t1 = omp_get_wtime();
    quickSort(arr, 0, N - 1);
    t2 = omp_get_wtime();
    printf("%f  it is 1 thread \n", t2-t1 );
    validateArray(N, arr);

    for(i = 0; i < N; i++) {
        arr[i] = rand() % 50;
    }

    t1 = omp_get_wtime();
    quickSortParallel(arr, 0, N, 1);
    t2 = omp_get_wtime();
    printf("%f it is multi tread\n", t2-t1);
    validateArray(N, arr);

    return 0;
}

