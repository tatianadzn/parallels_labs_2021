#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

double get_random(unsigned int *seed, int min, int max) {
    double random = ((double) rand_r(seed)) / (double) RAND_MAX;
    double diff = max - min;
    double r = random * diff;
    return min + r;
}

void array_copy(const double *original, double *copied, int size) {
    copied[0] = 0;
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

    unsigned int i, N, seed;

    if (argc != 2) {
        printf("Usage: ./lab1 N\n");
        printf("N - size of the array; should be greater than 2\n");
        return 1;
    }
    N = atoi(argv[1]);

    double *arr1 = malloc(sizeof(double) * N);
    double *arr2 = malloc(sizeof(double) * (N / 2));
    double *arr2_copy = malloc(sizeof(double) * (N / 2 + 1));


    /* 5 экспериментов */
    for (i = 0; i < 5; i++) {
        seed = i;

        /* Этап 1. Generate */
        for (int j = 0; j < N; j++) {
            arr1[j] = get_random(&seed, 1, A);
        }

        for (int j = 0; j < N / 2; j++) {
            arr2[j] = get_random(&seed, A, A * 10);
        }

        /* Этап 2. Map */
        // M1 (e -> hyperbolic cos(e) + 1)
        for (int j = 0; j < N; j++) {
            arr1[j] = cosh(arr1[j]) + 1;
        }

        // M2 -> M2_copy   {0, M2[0], M2[1]...}
        // e.g:
        // M2       : 1 2 3 4 5 6
        // M2_copy  : 0 1 2 3 4 5

        array_copy(arr2, arr2_copy, N / 2);

        // M2 (e -> abs(ctg( (e-1)+e )) )
        for (int j = 0; j < N / 2; j++) {
            arr2[j] = fabs(1 / tan(arr2[j] + arr2_copy[j]));
        }

        /* Этап 3. Merge */
        // M1, M2 (e1, e2 -> e1 / e2)
        for (int j = 0; j < N / 2; j++) {
            arr2[j] = arr1[j] / arr2[j];
        }

        /* Этап 4. Sort */
        combsort(arr2, N / 2);

        /* Этап 5. Reduce */
        double X = 0;

        for (int j = 0; j < N / 2; j++) {
            double min = find_min_in_sorted_arr(arr2, N / 2);
            if (min == 0) exit(0);

            if ((int) (arr2[j] / min) % 2 == 0) {
                X += sin(arr2[j]);
            }
        }

        printf("%d,%d,%f\n", N, i, X);
    }


    free(arr1);
    free(arr2);
    free(arr2_copy);



//    printf("N=%d. Milliseconds passed: %ld\n", N, delta_ms);
    return 0;
}

