#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#ifdef _OPENMP
#include "omp.h"
#else
void omp_set_num_threads(int M) { }

double omp_get_wtime() {
    struct timeval T1;
    gettimeofday(&T1, NULL);
    return (double) T1.tv_sec + (double) T1.tv_usec / 1000000;
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

int main(int argc, char *argv[]) {
    const int A = 8 * 7 * 10; // Дзензура Татьяна Михайловна

    unsigned int i, N, num_threads, seed;
    double T1, T2;
    long delta_ms;

    if (argc != 3) {
        printf("Usage: ./lab1 N num_threads\n");
        printf("N - size of the array; should be greater than 2\n");
        printf("num_threads - number of threads\n");
        return 1;
    }
    N = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    omp_set_num_threads(num_threads);

    double *arr1 = malloc(sizeof(double) * N);
    double *arr2 = malloc(sizeof(double) * (N / 2));
    double *arr2_copy = malloc(sizeof(double) * (N / 2 + 1));

    T1 = omp_get_wtime(); /* запомнить текущее время T1 */

    /* 100 экспериментов */
    for (i = 0; i < 100; i++) {
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
        combsort(arr2, N / 2);

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
    }

    T2 = omp_get_wtime(); /* запомнить текущее время T2 */

    free(arr1);
    free(arr2);
    free(arr2_copy);

    delta_ms = (T2 - T1) * 1000;

    printf("%d,%ld\n", N, delta_ms);
//    printf("N=%d. Milliseconds passed: %ld\n", N, delta_ms);
    return 0;
}
