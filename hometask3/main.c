#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>



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

void quickSortParallel(int* arr, int first, int last)
{

    int i = first, j = last, x = arr[(first + last) / 2];
    omp_set_nested(1);
    omp_set_max_active_levels(2);
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
    if((last - first > 1000) && (omp_get_active_level() < omp_get_max_active_levels())) {
        #pragma omp parallel sections
        {
            #pragma omp section
            quickSortParallel(arr, i, last);
            #pragma omp section
            quickSortParallel(arr, first, j);
        }
    } else {
        if (i < last)
            quickSortParallel(arr, i, last);
        if (first < j)
            quickSortParallel(arr, first, j);
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
    int N = 60000000;
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
    quickSortParallel(arr, 0, N);
    t2 = omp_get_wtime();
    printf("%f it is multi tread\n", t2-t1);
    validateArray(N, arr);

    return 0;
}

