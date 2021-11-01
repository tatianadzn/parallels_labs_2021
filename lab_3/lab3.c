#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

double get_random(unsigned int *seed, int min, int max) {
    double random = ((double) rand_r(seed)) / (double) RAND_MAX;
    double diff = max - min;
    double r = random * diff;
    return min + r;
}

void array_copy(const double *original, double *copied, int size) {
    copied[0] = 0;

    #pragma omp parallel for default(none) private(i)
    for (int i = 0; i < size; i++) {
        copied[i + 1] = original[i];
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

    unsigned int i, N, num_threads, schedule_par, chunk_size, seed;
    struct timeval T1, T2;
    long delta_ms;

    if (argc != 5) {
        printf("Usage: ./lab1 N num_threads schedule_par chunk_size\n");
        printf("N - size of the array; should be greater than 2\n");
        printf("num_threads - number of threads\n");
        printf("schedule_par - 0 for non, 1 for static, 2 for dynamic, 3 for guided\n");
        printf("chunk_size - parameter for schedule\n");
        return 1;
    }
    N = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    schedule_par = atoi(argv[3]);
    chunk_size = atoi(argv[4]);

    double *arr1 = malloc(sizeof(double) * N);
    double *arr2 = malloc(sizeof(double) * (N / 2));
    double *arr2_copy = malloc(sizeof(double) * (N / 2 + 1));

    gettimeofday(&T1, NULL); /* запомнить текущее время T1 */

    /* 100 экспериментов */
    omp_set_num_threads(num_threads)
    for (i = 0; i < 100; i++) {
        seed = i;

        /* Этап 1. Generate */
        #pragma omp parallel for default(none) private(j)
        if (schedule_par == 1) {
            #pragma omp for schedule(static, chunk_size)
        } else if (schedule_par == 2) {
            #pragma omp for schedule(dynamic, chunk_size)
        } else if (schedule_par == 3) {
            #pragma omp for schedule(guided, chunk_size)
        }
        for (int j = 0; j < N; j++) {
            arr1[j] = get_random(&seed, 1, A);
        }

        #pragma omp parallel for default(none) private(j)
        if (schedule_par == 1) {
            #pragma omp for schedule(static, chunk_size)
        } else if (schedule_par == 2) {
            #pragma omp for schedule(dynamic, chunk_size)
        } else if (schedule_par == 3) {
            #pragma omp for schedule(guided, chunk_size)
        }
        for (int j = 0; j < N / 2; j++) {
            arr2[j] = get_random(&seed, A, A * 10);
        }

        /* Этап 2. Map */
        // M1 (e -> hyperbolic cos(e) + 1)
        #pragma omp parallel for default(none) private(j)
        if (schedule_par == 1) {
            #pragma omp for schedule(static, chunk_size)
        } else if (schedule_par == 2) {
            #pragma omp for schedule(dynamic, chunk_size)
        } else if (schedule_par == 3) {
            #pragma omp for schedule(guided, chunk_size)
        }
        for (int j = 0; j < N; j++) {
            arr1[j] = cosh(arr1[j]) + 1;
        }

        // M2 -> M2_copy   {0, M2[0], M2[1]...}
        // e.g:
        // M2       : 1 2 3 4 5 6
        // M2_copy  : 0 1 2 3 4 5

        array_copy(arr2, arr2_copy, N / 2);

        // M2 (e -> abs(ctg( (e-1)+e )) )
        #pragma omp parallel for default(none) private(j)
        if (schedule_par == 1) {
            #pragma omp for schedule(static, chunk_size)
        } else if (schedule_par == 2) {
            #pragma omp for schedule(dynamic, chunk_size)
        } else if (schedule_par == 3) {
            #pragma omp for schedule(guided, chunk_size)
        }
        for (int j = 0; j < N / 2; j++) {
            arr2[j] = fabs(1 / tan(arr2[j] + arr2_copy[j]));
        }

        /* Этап 3. Merge */
        // M1, M2 (e1, e2 -> e1 / e2)
        #pragma omp parallel for default(none) private(j)
        if (schedule_par == 1) {
            #pragma omp for schedule(static, chunk_size)
        } else if (schedule_par == 2) {
            #pragma omp for schedule(dynamic, chunk_size)
        } else if (schedule_par == 3) {
            #pragma omp for schedule(guided, chunk_size)
        }
        for (int j = 0; j < N / 2; j++) {
            arr2[j] = arr1[j] / arr2[j];
        }

        /* Этап 4. Sort */
        combsort(arr2, N / 2);

        /* Этап 5. Reduce */
        double X = 0;

        #pragma omp parallel for default(none) private(j) shared(X)
        if (schedule_par == 1) {
            #pragma omp for schedule(static, chunk_size)
        } else if (schedule_par == 2) {
            #pragma omp for schedule(dynamic, chunk_size)
        } else if (schedule_par == 3) {
            #pragma omp for schedule(guided, chunk_size)
        }
        for (int j = 0; j < N / 2; j++) {
            double min = find_min_in_sorted_arr(arr2, N / 2);
            if (min == 0) exit(0);

            if ((int) (arr2[j] / min) % 2 == 0) {
                #pragma omp critical
                X += sin(arr2[j]);
            }
        }
    }

    gettimeofday(&T2, NULL); /* запомнить текущее время T2 */

    free(arr1);
    free(arr2);
    free(arr2_copy);

    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;

    printf("%d,%ld\n", N, delta_ms);
//    printf("N=%d. Milliseconds passed: %ld\n", N, delta_ms);
    return 0;
}
