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

struct thread_params {
    int chunk_size;
    int thr_id;
    int num_threads;
};

struct map_parameters {
    unsigned int array_size;
    double *array;
    struct thread_params thread_p;
};

struct generate_array_params {
    double *array;
    int size;
    unsigned int *seed;
    int min;
    int max;
    struct thread_params thread_p;
};

struct copy_parameters {
    double *original; double *copied; int size;
    struct thread_params thread_p;
};

struct map_two_parameters {
    unsigned int N;
    double *arr2;
    double *arr2_copy;
    struct thread_params thread_p;
};

struct sort_params {
    double *arr;
    unsigned int size;
};

struct reduce_parameters {
    unsigned int N;
    double *arr2;
    double min;
    double res;
    struct thread_params thread_p;
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

void *generate_array(void *gen_arr_params_v) {
    struct generate_array_params *gen_arr_params = (struct generate_array_params*) gen_arr_params_v;
    double *array = gen_arr_params->array;
    int size = gen_arr_params->size;
    unsigned int *seed = gen_arr_params->seed;
    int min = gen_arr_params->min;
    int max = gen_arr_params->max;
    int chunk = gen_arr_params->thread_p.chunk_size;
    int tid = gen_arr_params->thread_p.thr_id;
    int num_threads = gen_arr_params->thread_p.num_threads;

    for (int j = tid*chunk; j < size; j+=num_threads*chunk) {
        for (int i = 0; j+i < size && i < chunk; ++i) {
            int next = j+i;
            unsigned int seed_j = next + *seed;
            double random = ((double) rand_r(&seed_j)) / (double) RAND_MAX;
            double diff = max - min;
            double r = random * diff;
            array[next] = min + r;
            // printf("t_id=%d j=%d array[j]=%f\n", tid, next, array[next]);
        }
    }
    pthread_exit(NULL);
}

void generate_array_pthreads(
        double *array,
        int size,
        unsigned int *seed,
        int min,
        int max,
        int chunk_size,
        int num_threads
) {
    struct generate_array_params gen_arr_params[num_threads];
    pthread_t threads[num_threads];
    for (int j = 0; j < num_threads; ++j) {
        gen_arr_params[j].array = array;
        gen_arr_params[j].size = size;
        gen_arr_params[j].seed = seed;
        gen_arr_params[j].min = min;
        gen_arr_params[j].max = max;
        gen_arr_params[j].thread_p.chunk_size = chunk_size;
        gen_arr_params[j].thread_p.thr_id = j;
        gen_arr_params[j].thread_p.num_threads = num_threads;
        pthread_create(&threads[j], NULL, generate_array, &gen_arr_params[j]);
    }
    for (int j = 0; j < num_threads; ++j) pthread_join(threads[j], NULL);
}

void *a_copy_pthread(void *params) {
    struct copy_parameters *p = (struct copy_parameters*) params;
    double *original = p->original;
    double *copied = p->copied;
    int size = p->size;
    int chunk = p->thread_p.chunk_size;
    int tid = p->thread_p.thr_id;
    int num_threads = p->thread_p.num_threads;

    for (int j = tid*chunk; j < size; j+=num_threads*chunk) {
        for (int i = 0; j+i < size && i < chunk; ++i) {
            int next = j + i;
            copied[next] = original[next];
            // printf("tid=%d i=%d copied[i]=%f=original[i]=%f\n", tid, next, copied[next], original[next]);
        }
    }
    //printf("...\n");
    pthread_exit(NULL);
}

void a_copy(double *original, double *copied, int size, int num_threads) {
    struct copy_parameters mp[num_threads];
    pthread_t threads[num_threads];
    for (int j = 0; j < num_threads; ++j) {
        mp[j].original = original;
        mp[j].copied = copied;
        mp[j].size = size;
        mp[j].thread_p.chunk_size = size / num_threads;
        mp[j].thread_p.thr_id = j;
        mp[j].thread_p.num_threads = num_threads;
        pthread_create(&threads[j], NULL, a_copy_pthread, &mp[j]);
    }
    for (int j = 0; j < num_threads; ++j) pthread_join(threads[j], NULL);
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

void *combsort_pthreads(void *params) {
    struct sort_params *p = (struct sort_params*) params;
    double *arr = p->arr;
    int size = p->size;

    combsort(arr, size);

    pthread_exit(NULL);
}

double find_min_in_sorted_arr(double *sorted_arr, int size) {
    for (int i = 0; i < size; i++) {
        if (sorted_arr[i] != 0) {
            return sorted_arr[i];
        }
    }
    return 0;
}

void *map_pthreads(void *params) {
    struct map_parameters *p = (struct map_parameters*) params;
    unsigned int N = p->array_size;
    double *arr1 = p->array;
    int chunk = p->thread_p.chunk_size;
    int tid = p->thread_p.thr_id;
    int num_threads = p->thread_p.num_threads;

    for (int j = tid*chunk; j < N; j+=num_threads*chunk) {
        for (int i = 0; j+i < N && i < chunk; ++i) {
            int next = j + i;
            arr1[next] = cosh(arr1[next]) + 1;
            //printf("tid=%d j=%d arr1[j]=%f\n", tid, next, arr1[next]);
        }
    }
    //printf("...\n");
    pthread_exit(NULL);
}

void *map_two_pthreads(void *params) {
    struct map_two_parameters *p = (struct map_two_parameters*) params;
    unsigned int N = p->N;
    double *arr2 = p->arr2;
    double *arr2_copy = p->arr2_copy;
    int chunk = p->thread_p.chunk_size;
    int tid = p->thread_p.thr_id;
    int num_threads = p->thread_p.num_threads;

    for (int j = tid*chunk; j < N; j+=num_threads*chunk) {
        for (int i = 0; j+i < N && i < chunk; ++i) {
            int next = j + i;
            arr2[next] = fabs(1 / tan(arr2[next] + arr2_copy[next]));
            // printf("tid=%d j=%d arr2[j]=%f\n",tid, next, arr2[next]);
        }
    }
    //printf("...\n");
    pthread_exit(NULL);
}

void *merge_pthreads(void *params) {
    struct map_two_parameters *p = (struct map_two_parameters*) params;
    unsigned int N = p->N;
    double *arr2 = p->arr2;
    double *arr2_copy = p->arr2_copy;
    int chunk = p->thread_p.chunk_size;
    int tid = p->thread_p.thr_id;
    int num_threads = p->thread_p.num_threads;

    for (int j = tid*chunk; j < N; j+=num_threads*chunk) {
        for (int i = 0; j+i < N && i < chunk; ++i) {
            int next = j + i;
            arr2_copy[next] = arr2[next] / arr2_copy[next];
            // printf("tid=%d j=%d arr2[j]=%f\n", tid, j, arr2[j]);
        }
    }
    //printf("...\n");
    pthread_exit(NULL);
}

void *reduce_pthreads(void *params) {
    struct reduce_parameters *p = (struct reduce_parameters*) params;
    unsigned int N = p->N;
    double *arr2 = p->arr2;
    double min = p->min;
    double res = p->res;
    int chunk = p->thread_p.chunk_size;
    int tid = p->thread_p.thr_id;
    int num_threads = p->thread_p.num_threads;

    for (int j = tid*chunk; j < N; j+=num_threads*chunk) {
        for (int i = 0; j+i < N && i < chunk; ++i) {
            int next = j + i;
            if ((int) (arr2[next] / min) % 2 == 0) {
                res += sin(arr2[next]);
            }
        }
    }
    // printf("tid=%d res=%f\n", tid, res);
    p->res = res;
    pthread_exit(NULL);
}

void *main_logic(void *params_p) {
    const int A = 8 * 7 * 10; // Дзензура Татьяна Михайловна

    unsigned int i, N, seed;
    double T1, T2;
    long delta_ms;

    // int argc, char *argv[], int *progress
    struct main_logic_parameters *params = (struct main_logic_parameters *) params_p;
    N = params->N;
    unsigned num_threads = params->num_threads;
    int* progress = params->progress;

    double *arr1 = malloc(sizeof(double) * N);
    double *arr2 = malloc(sizeof(double) * (N / 2));
    double *arr2_copy = malloc(sizeof(double) * (N / 2 + 1));
    double *arr2_sorted = malloc(sizeof(double) * (N / 2));

    T1 = get_time(); /* запомнить текущее время T1 */

    /* 100 экспериментов */
    int experiments_count = 1;
    for (i = 0; i < experiments_count; i++) {
        seed = i;

        /* Этап 1. Generate */
        generate_array_pthreads(arr1, N, &seed, 1, A, N/num_threads, num_threads);
        generate_array_pthreads(arr2, N / 2, &seed, 1, A, N/2/num_threads, num_threads);

        /* Этап 2. Map */
        // M1 (e -> hyperbolic cos(e) + 1)
        //printf("--- map: hyperbolic cos(e) + 1\n");
        struct map_parameters mp[num_threads];
        pthread_t threads[num_threads];
        for (int j = 0; j < num_threads; ++j) {
            mp[j].array = arr1;
            mp[j].array_size = N;
            mp[j].thread_p.chunk_size = N / num_threads;
            mp[j].thread_p.thr_id = j;
            mp[j].thread_p.num_threads = num_threads;
            pthread_create(&threads[j], NULL, map_pthreads, &mp[j]);
        }
        for (int j = 0; j < num_threads; ++j) pthread_join(threads[j], NULL);

        // M2 -> M2_copy   {0, M2[0], M2[1]...}
        // e.g:
        // M2       : 1 2 3 4 5 6
        // M2_copy  : 0 1 2 3 4 5
        arr2_copy[0] = 0;
        a_copy(arr2, arr2_copy + 1, N / 2, num_threads);

        // M2 (e -> abs(ctg( (e-1)+e )) )
        struct map_two_parameters mp_t[num_threads];
        pthread_t threads_two[num_threads];
        for (int j = 0; j < num_threads; ++j) {
            mp_t[j].N = N / 2;
            mp_t[j].arr2 = arr2;
            mp_t[j].arr2_copy = arr2_copy;
            mp_t[j].thread_p.chunk_size = N / 2 / num_threads;
            mp_t[j].thread_p.thr_id = j;
            mp_t[j].thread_p.num_threads = num_threads;
            pthread_create(&threads_two[j], NULL, map_two_pthreads, &mp_t[j]);
        }
        for (int j = 0; j < num_threads; ++j) pthread_join(threads_two[j], NULL);

        /* Этап 3. Merge */
        // M1, M2 (e1, e2 -> e1 / e2)
        for (int j = 0; j < num_threads; ++j) {
            mp_t[j].arr2 = arr1;
            mp_t[j].arr2_copy = arr2;
            pthread_create(&threads_two[j], NULL, merge_pthreads, &mp_t[j]);
        }
        for (int j = 0; j < num_threads; ++j) pthread_join(threads_two[j], NULL);

        /* Этап 4. Sort */
        /*printf("%d %d \n", N, N/2);
        for(int j = 0; j < N / 2; j++) {
            printf("arr2[%d] = %f\n", j, arr2[j]);
        }
        printf("\n");*/
        pthread_t threads_sort[2];
        struct sort_params sp[2];
        sp[0].arr = arr2;
        sp[0].size = N / 4;
        pthread_create(&threads_sort[0], NULL, combsort_pthreads, &sp[0]);
        sp[1].arr = arr2 + N / 4;
        sp[1].size = N / 2 - N / 4;
        pthread_create(&threads_sort[1], NULL, combsort_pthreads, &sp[1]);
        pthread_join(threads_sort[0], NULL);
        pthread_join(threads_sort[1], NULL);
        join_section_arrays(arr2_sorted, arr2, N / 4, arr2 + N / 4, N / 2 - N / 4);
        a_copy(arr2_sorted, arr2, N / 2, num_threads);
        /*for(int j = 0; j < N / 2; j++) {
            printf("arr2[%d] = %f\n", j, arr2[j]);
        }*/

        /* Этап 5. Reduce */
        double X = 0;
        double min = find_min_in_sorted_arr(arr2, N / 2);
        if (min == 0) exit(0);
        struct reduce_parameters rp[num_threads];
        pthread_t rp_threads[num_threads];
        for (int j = 0; j < num_threads; ++j) {
            rp[j].N = N / 2;
            rp[j].arr2 = arr2;
            rp[j].min = min;
            rp[j].res = 0;
            rp[j].thread_p.chunk_size = N / 2 / num_threads;
            rp[j].thread_p.thr_id = j;
            rp[j].thread_p.num_threads = num_threads;
            pthread_create(&rp_threads[j], NULL, reduce_pthreads, &rp[j]);
        }
        for (int j = 0; j < num_threads; ++j) {
            pthread_join(rp_threads[j], NULL);
            X += rp[j].res;
        }
        // printf("res=%f\n", X);
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
