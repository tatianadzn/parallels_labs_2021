#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

struct main_logic_parameters {
    int N;
    int num_threads;
    int* progress;
};

void *progress_notifier(void *progress_p) {
    int *progress = (int *) progress_p;
    int value;
    for(;;) {
        value = *progress;
        // printf("progress: %d\n", value);
        if (value >= 100) break;
        sleep(1);
    }
    pthread_exit(NULL);
}

double get_time() {
    struct timeval T1;
    gettimeofday(&T1, NULL);
    return (double) T1.tv_sec + (double) T1.tv_usec / 1000000;
}

#ifdef _OPENMP
#include "omp.h"

void join_section_arrays(double *res_array, double *array1, int size1, double *array2, int size2) {
    /*printf("s1 = %d; s2 = %d\n", size1, size2);
    for(int j = 0; j < size1; j++) {
        printf("arr1[%d] = %f\n", j, array1[j]);
    }
    for(int j = 0; j < size2; j++) {
        printf("arr2[%d] = %f\n", j, array2[j]);
    }*/
    int i1, i2, i_res;
    i1 = 0;
    i2 = 0;
    i_res = 0;

    for(; i1 < size1 && i2 < size2;)
        if (array1[i1] < array2[i2])    res_array[i_res++] = array1[i1++];
        else                            res_array[i_res++] = array2[i2++];

    while (i1 < size1)  res_array[i_res++] = array1[i1++];
    while (i2 < size2)  res_array[i_res++] = array2[i2++];
}
#endif

void generate_array(double *array, int size, unsigned int *seed, int min, int max) {
    //printf("--- generate:\n");
    #pragma omp parallel for default(none) shared(array, size, seed, min, max)
    for (int j = 0; j < size; j++) {
        unsigned int seed_j = j + *seed;
        double random = ((double) rand_r(&seed_j)) / (double) RAND_MAX;
        double diff = max - min;
        double r = random * diff;
        array[j] = min + r;
        //printf("j=%d array[j]=%f\n", j, array[j]);
    }
}

void array_copy(const double *original, double *copied, int size) {
    copied[0] = 0;
    //printf("--- map: copy\n");
    #pragma omp parallel for default(none) shared(size, original, copied)
    for (int i = 0; i < size; i++) {
        copied[i + 1] = original[i];
        //printf("i=%d copied[i+1]=%f=original[i]=%f\n", i, copied[i+1], original[i]);
    }
    //printf("...\n");
}

void a_copy(const double *original, double *copied, int size) {
    #pragma omp parallel for default(none) shared(size, original, copied)
    for (int i = 0; i < size; i++) {
        copied[i] = original[i];
        //printf("i=%d copied[i+1]=%f=original[i]=%f\n", i, copied[i+1], original[i]);
    }
}

/* Combsort: function to find the new gap between the elements */
int newgap(int gap) {
    gap = (gap * 10) / 13;
    if (gap == 9 || gap == 10)
        gap = 11;
    if (gap < 1)
        gap = 1;
    return gap;
}

/* Combsort: implementation */
void combsort(double a[], int aSize) {
    /*printf("a[0] = %f\n", a[0]);
    printf("a[aSize-1] = %f\n", a[aSize-1]);*/
    int gap = aSize;
    double temp;
    int i;
    for (;;) {
        gap = newgap(gap);
        int swapped = 0;
        for (i = 0; i < aSize - gap; i++) {
            int j = i + gap;
            if (a[i] > a[j]) {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
                swapped = 1;
            }
        }
        if (gap == 1 && !swapped)
            break;
    }
}

double find_min_in_sorted_arr(double *sorted_arr, int size) {
    for (int i = 0; i < size; i++) {
        if (sorted_arr[i] != 0) {
            return sorted_arr[i];
        }
    }
    return 0;
}

void *main_logic(void *params_p) {
    const int A = 8 * 7 * 10; // Дзензура Татьяна Михайловна

    unsigned int i, N, seed;
    double T1, T2;
    long delta_ms;

    // int argc, char *argv[], int *progress
    struct main_logic_parameters *params = (struct main_logic_parameters *) params_p;
    N = params->N;
    // unsigned num_threads = params->num_threads;
    int* progress = params->progress;

    double *arr1 = malloc(sizeof(double) * N);
    double *arr2 = malloc(sizeof(double) * (N / 2));
    double *arr2_copy = malloc(sizeof(double) * (N / 2 + 1));
    double *arr2_sorted = malloc(sizeof(double) * (N / 2));

    T1 = get_time(); /* запомнить текущее время T1 */

    /* 100 экспериментов */
    int experiments_count = 100;
    for (i = 0; i < experiments_count; i++) {
        seed = i;

        /* Этап 1. Generate */
        generate_array(arr1, N, &seed, 1, A);
        generate_array(arr2, N / 2, &seed, 1, A);

        /* Этап 2. Map */
        // M1 (e -> hyperbolic cos(e) + 1)
        //printf("--- map: hyperbolic cos(e) + 1\n");
        #pragma omp parallel for default(none) shared(N, arr1)
        for (int j = 0; j < N; j++) {
            arr1[j] = cosh(arr1[j]) + 1;
            //printf("j=%d arr1[j]=%f\n", j, arr1[j]);
        }
        //printf("...\n");

        // M2 -> M2_copy   {0, M2[0], M2[1]...}
        // e.g:
        // M2       : 1 2 3 4 5 6
        // M2_copy  : 0 1 2 3 4 5

        array_copy(arr2, arr2_copy, N / 2);

        // M2 (e -> abs(ctg( (e-1)+e )) )
        //printf("--- map: abs(ctg( (e-1)+e )\n");
        #pragma omp parallel for default(none) shared(N, arr2, arr2_copy)
        for (int j = 0; j < N / 2; j++) {
            arr2[j] = fabs(1 / tan(arr2[j] + arr2_copy[j]));
            //printf("j=%d arr2[j]=%f\n", j, arr2[j]);
        }
        //printf("...\n");

        /* Этап 3. Merge */
        // M1, M2 (e1, e2 -> e1 / e2)
        //printf("--- merge: e1 / e2\n");
        #pragma omp parallel for default(none) shared(N, arr1, arr2)
        for (int j = 0; j < N / 2; j++) {
            arr2[j] = arr1[j] / arr2[j];
            //printf("j=%d arr2[j]=%f\n", j, arr2[j]);
        }
        //printf("...\n");

        /* Этап 4. Sort */
        /*printf("%d %d \n", N, N/2);
        for(int j = 0; j < N / 2; j++) {
            printf("arr2[%d] = %f\n", j, arr2[j]);
        }*/
        #ifdef _OPENMP
            /*printf("\n");*/
            #pragma omp parallel sections
            {
                #pragma omp section
                combsort(arr2, N / 4);
                #pragma omp section
                combsort(arr2 + N / 4, N / 2 - N / 4);
            }
            join_section_arrays(arr2_sorted, arr2, N / 4, arr2 + N / 4, N / 2 - N / 4);
            a_copy(arr2_sorted, arr2, N / 2);
            /*for(int j = 0; j < N / 2; j++) {
                printf("arr2[%d] = %f\n", j, arr2_sorted[j]);
            }*/
        #else
            combsort(arr2, N / 2);
            /*for(int j = 0; j < N / 2; j++) {
                printf("arr2[%d] = %f\n", j, arr2[j]);
            }*/
        #endif
        /* Этап 5. Reduce */
        double X = 0;
        double min = find_min_in_sorted_arr(arr2, N / 2);
        if (min == 0) exit(0);
        //printf("--- reduce: + sin\n");
        #pragma omp parallel for default(none) shared(N, arr2, min) reduction(+:X)
        for (int j = 0; j < N / 2; j++) {
            if ((int) (arr2[j] / min) % 2 == 0) {
                X += sin(arr2[j]);
            }
        }
        //printf("X=%f\n", X);
        //printf("...\n");

        *progress = (100 * (i + 1)) / experiments_count;
    }

    T2 = get_time(); /* запомнить текущее время T2 */

    free(arr1);
    free(arr2);
    free(arr2_copy);
    free(arr2_sorted);

    delta_ms = (T2 - T1) * 1000;

    printf("%d,%ld\n", N, delta_ms);
//    printf("N=%d. Milliseconds passed: %ld\n", N, delta_ms);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int *progress = malloc(sizeof(int));
    *progress = 0;
    pthread_t threads[2];
    struct main_logic_parameters params;

    if (argc != 3) {
        printf("Usage: ./lab5 N num_threads\n");
        printf("N - size of the array; should be greater than 2\n");
        printf("num_threads - number of threads\n");
        return 1;
    }
    params.N = atoi(argv[1]);
    params.num_threads = atoi(argv[2]);
    params.progress = progress;

    pthread_create(&threads[0], NULL, progress_notifier, progress);
    pthread_create(&threads[1], NULL, main_logic, &params);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    free(progress);
}
